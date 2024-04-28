#include "job-system.h"
#include "../analysis/logger.h"
#include "../analysis/debug.h"
#include "../../thirdparty/src/optick.h"

#include "scoped-spinlock.h"
#include "spinlock.h"
#include "scoped-mutex.h"

#include <queue>
#include <mutex>

namespace CCE::Jobs
{
	
#define NUM_FIBERS 150

	HANDLE mainThread{};
	LPVOID mainFiber{};

	std::vector<std::thread*> worker_threads = {};
	std::vector<LPVOID> thread_fibers = {};

	std::deque<Job> job_queue_high{};
	std::deque<Job> job_queue_normal{};
	std::deque<Job> job_queue_low{};

	std::queue<LPVOID> fiber_pool{};

	std::atomic<bool> runThreads(true);
	
	CCE::SpinLock job_queue_sl{};
	CCE::SpinLock fiber_pool_sl{};
	CCE::SpinLock wait_list_sl{};

	struct WaitData
	{
		LPVOID m_Fiber = nullptr;
		Counter* m_pCounter = nullptr;
		int m_desiredCount = 0;

		WaitData(const LPVOID _fiber, Counter* _counter, const int desiredCount)
			: m_Fiber(_fiber), m_pCounter(_counter), m_desiredCount(desiredCount)
		{ }

		~WaitData() = default;

		bool operator !=(const WaitData& other)
		{
			return m_Fiber != other.m_Fiber;
		}

		bool operator ==(const WaitData& other)
		{
			return m_Fiber == other.m_Fiber;
		}
	};

	std::vector<WaitData> wait_list{};
	

	void DeinitializeThreadpool()
	{
		runThreads.store(false, std::memory_order_release);

		Sleep(1);

		{
			ScopedSpinLock lock(fiber_pool_sl);
			while (fiber_pool.size() > 0)
			{
				LPVOID fiber = fiber_pool.front();
				fiber_pool.pop();
				DeleteFiber(fiber);
			}
		}

		{
			ScopedSpinLock lock(wait_list_sl);
			for(int i = 0; i < wait_list.size(); ++i)
			{
				DeleteFiber(wait_list[i].m_Fiber);
			}
		}
	}

	Jobs::Job GetNextJob()
	{
		OPTICK_EVENT();
		ScopedSpinLock lock(job_queue_sl);
		Jobs::Job jobCpy;

		if (!job_queue_high.empty())
		{
			jobCpy = std::move(job_queue_high.front());
			job_queue_high.pop_front();
			return jobCpy;
		}

		if (!job_queue_normal.empty())
		{
			jobCpy = std::move(job_queue_normal.front());
			job_queue_normal.pop_front();
			return jobCpy;
		}

		if (!job_queue_low.empty())
		{
			jobCpy = std::move(job_queue_low.front());
			job_queue_low.pop_front();
			return jobCpy;
		}
	}

	/// <summary>
	/// Checks the waitlist for waiting jobs and returns to those jobs that are done waiting.
	/// </summary>
	void CheckWaitList()
	{
		OPTICK_EVENT();
		wait_list_sl.Acquire();
		if (!wait_list.empty())
		{
			for (auto it = wait_list.begin(); it != wait_list.end(); ++it)
			{
				if (*(it->m_pCounter) <= it->m_desiredCount)	// Error? Invalid iterator? --> See below!
				{
					// If job is ready, remove wait data entry, return fiber and switch to waiting fiber!
					fiber_pool_sl.Acquire();
					LPVOID fiberToSwitchTo = it->m_Fiber;	// Error? Invalid iterator? --> Probably wrong TLS

					wait_list.erase(it);
					wait_list_sl.Release();
					fiber_pool.push(GetCurrentFiber());
					fiber_pool_sl.Release();
					SwitchToFiber(fiberToSwitchTo);
					break; // Break in order to make this work with multiple wait list elements!

					// Erasing an element from wait_list may result in an error when returning to this job
					// and trying to iterate using the old iterator.
				}
			}
		}
		wait_list_sl.Release();
	}

	VOID RunFiber()
	{
		while (runThreads)
		{
			OPTICK_EVENT("FiberLoop");
			
			CheckWaitList();

			Job jobCpy = GetNextJob();

			if (jobCpy.m_EntryPoint != nullptr)
			{
				// Valid job
				
				// New job -> Get new fiber! | Job will not have a fiber associated with it yet since this case 
				// is handled before!
				jobCpy.m_Fiber = GetCurrentFiber();
				jobCpy.m_EntryPoint(jobCpy.m_Param);
				jobCpy.m_pCounter->fetch_sub(1);
				jobCpy.m_Fiber = nullptr;
			}
		}

		// Do not terminate the main thread!
		if(GetCurrentThread() != mainThread)
			DeleteFiber(GetCurrentFiber());
	}

	LPVOID GetFiber()
	{
		OPTICK_EVENT();
		LPVOID fiber;
		// Critical section!
		{
			CCE::ScopedSpinLock lock(fiber_pool_sl);
			DASSERT(fiber_pool.size() != 0, "Job system ran out of fibers!");

			fiber = fiber_pool.front();
			fiber_pool.pop();
		}
		return fiber;
	}

	void RunThread()
	{
		OPTICK_THREAD("WORKER");
		LPVOID threadFiber = ConvertThreadToFiber(0);
		SwitchToFiber(GetFiber());
		LOG("Terminated Thread %d", GetCurrentThreadId());
	}

	void InitializeThreadpool(int numOfThreads)
	{
		unsigned int hardwareThreads = std::thread::hardware_concurrency();

		// Define number of threads
		if (numOfThreads < 1 || numOfThreads > hardwareThreads)
			numOfThreads = hardwareThreads - 1;

		LOG_JOBS("Number of logical cpu cores: %i", std::thread::hardware_concurrency());
		LOG_JOBS("Number of worker threads: %i", numOfThreads);
		worker_threads.reserve(numOfThreads);
		thread_fibers.reserve(numOfThreads);

		mainThread = GetCurrentThread();
		mainFiber = ConvertThreadToFiber(0);
		std::thread* workerThread = nullptr;
		runThreads.store(true, std::memory_order_relaxed);

		for (int i = 0; i < NUM_FIBERS; ++i)
		{
			LPVOID fiber = CreateFiber(0, (LPFIBER_START_ROUTINE) &RunFiber, NULL);
			fiber_pool.push(fiber);
		}

		wait_list.reserve(fiber_pool.size());

		// Create worker threads
		for (unsigned short t_index = 0; t_index < numOfThreads; ++t_index)
		{
			// Spawn threads
			workerThread = new std::thread(RunThread);
			auto hndl = workerThread->native_handle();

			// Add to list
			worker_threads.push_back(workerThread);
		}
	}

	void ReturnFiber(LPVOID fiber)
	{
		// Critical section!
		{
			CCE::ScopedSpinLock lock(fiber_pool_sl);
			fiber_pool.push(fiber);
		}
	}

	void KickJob(Job job)
	{
		OPTICK_EVENT();
		ScopedSpinLock lock(job_queue_sl);

		switch (job.m_Priority)
		{
		case Priority::HIGH:
			job_queue_high.push_back(std::move(job));
			break;
		case Priority::NORMAL:
			job_queue_normal.push_back(std::move(job));
			break;
		case Priority::LOW:
			job_queue_low.push_back(std::move(job));
			break;
		}
	}

	__forceinline void BusyWaitForCounter(Counter* const cnt, const int desiredCount)
	{
		while (cnt->load(std::memory_order_consume) > desiredCount)
		{
			//// Put on wait list!
			//{
			//	ScopedSpinLock lock(wait_list_sl);
			//	wait_list.push_back(WaitData(GetCurrentFiber(), cnt, desiredCount));
			//}

			//// Switch to new fiber
			//SwitchToFiber(GetFiber());
		}
	}			// Error? --> Probably wrong stack memory because fiber woke up on different thread?
}

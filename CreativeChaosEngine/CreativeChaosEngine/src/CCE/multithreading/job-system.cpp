#include "job-system.h"
#include "../analysis/logger.h"
#include "../analysis/debug.h"
#include "../../thirdparty/src/optick.h"

#include "scoped-spinlock.h"
#include "spinlock.h"
#include "scoped-mutex.h"

#include <thread>
#include <queue>
#include <mutex>
#include <unordered_map>

namespace CCE::Jobs
{
	
#define NUM_FIBERS 150

#if NUM_FIBERS > 2028 
#error There can only be a maximum of 2028 fibers present at the same time!
#endif

	HANDLE mainThread{};
	LPVOID mainFiber{};
	
	std::atomic<bool> runThreads(true);

	std::vector<std::thread*> worker_threads = {};
	std::unordered_map<HANDLE, LPVOID> thread_fibers = {};

	std::deque<Job> job_queue_high{};
	std::deque<Job> job_queue_normal{};
	std::deque<Job> job_queue_low{};

	std::queue<LPVOID> fiber_pool{};

	CCE::SpinLock thread_fibers_sl{};
	CCE::SpinLock job_queue_sl{};
	CCE::SpinLock fiber_pool_sl{};
	CCE::SpinLock wait_list_sl{};
	CCE::SpinLock schedule_list_sl{};

	struct WaitData
	{
		LPVOID m_Fiber = nullptr;
		Counter* m_pCounter = nullptr;
		int m_desiredCount = 0;

		WaitData()
			: m_Fiber(nullptr), m_pCounter(nullptr), m_desiredCount(0)
		{ }

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
	std::unordered_map<LPVOID, WaitData> schedule_list{};
	

	void DeinitializeThreadpool()
	{
		runThreads.store(false, std::memory_order_release);

		Sleep(1);

		ConvertFiberToThread(); // @TODO: Which thread will this be and will it be joined soon?

		// Join and clear the worker threads
		{
			for (auto thread : worker_threads)
			{
				if (thread->joinable())
				{
					thread->join();
				}
			}

			worker_threads.clear();
		}
		
		// Clear the fiber pool and delete all fibers in the pool
		{
			ScopedSpinLock lock(fiber_pool_sl);
			while (fiber_pool.size() > 0)
			{
				LPVOID fiber = fiber_pool.front();
				fiber_pool.pop();
				DeleteFiber(fiber);
			}
		}

		// Clear the wait list and delete all fibers in the wait list
		{
			ScopedSpinLock lock(wait_list_sl);
			for(int i = 0; i < wait_list.size(); ++i)
			{
				DeleteFiber(wait_list[i].m_Fiber);
			}

			wait_list.clear();
		}

		// Delete all scheduled fibers and clear the list
		{
			ScopedSpinLock lock(schedule_list_sl);
			for (auto kvp : schedule_list)
			{
				DeleteFiber(kvp.second.m_Fiber);
			}
			schedule_list.clear();
		}
		
		// Delete the fibers which were created on the threads initially
		{
			ScopedSpinLock lock(thread_fibers_sl);

			for (auto kvp : thread_fibers)
			{
				DeleteFiber(kvp.second);
			}
			
			thread_fibers.clear();
		}

		job_queue_high.clear();
		job_queue_normal.clear();
		job_queue_low.clear();


	}

	Jobs::Job GetNextJob()
	{
		OPTICK_EVENT();
		ScopedSpinLock lock(job_queue_sl);
		Jobs::Job jobCpy;

		OPTICK_TAG("JobQueueHigh", job_queue_high.size());
		OPTICK_TAG("JobQueueNormal", job_queue_normal.size());
		OPTICK_TAG("JobQueueLow", job_queue_low.size());

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
		OPTICK_TAG("WaitListSize", wait_list.size());

		bool waitListAlreadyReleased = false;

		if (!wait_list.empty())
		{
			bool isValidData = false;
			std::vector<WaitData>::iterator validWaitData = {};

			for (auto it = wait_list.begin(); it != wait_list.end(); ++it)
			{
				if (it->m_pCounter->load(std::memory_order_relaxed) <= it->m_desiredCount)
				{
					DASSERT(it->m_pCounter->load(std::memory_order_relaxed) <= it->m_desiredCount, 
						"Counter is not yet equal or less than the desired count!");
					
					isValidData = true;
					validWaitData = it;
					break; // Break in order to make this work with multiple wait list elements!
				}
			}

			// If job is ready, remove wait data entry, return fiber and switch to waiting fiber!
			if (isValidData)
			{
				LPVOID fiberToSwitchTo = validWaitData->m_Fiber;

				{
					ScopedSpinLock lock(fiber_pool_sl);
					
					wait_list.erase(validWaitData);
					wait_list_sl.Release();
					waitListAlreadyReleased = true;				// Store in FLS that we actually released the wait list spin lock!
					fiber_pool.push(GetCurrentFiber());
				}
				SwitchToFiber(fiberToSwitchTo);
			}
		}
		// If there was no job in the wait list we have to release the spin lock.
		// BUT: If we just came from a fiber we must not re-release the fiber! 
		// Otherwise there will be an invalid iterator somewhere else!
		if(!waitListAlreadyReleased)
			wait_list_sl.Release();
	}

	VOID RunFiber()
	{
		while (runThreads)
		{
			{
				OPTICK_EVENT();

				UpdateWaitData();	// @TODO: Try to somehow do this more elegantly!!

				CheckWaitList();

				if (job_queue_high.empty() && job_queue_normal.empty() && job_queue_low.empty())
					continue;

				Job jobCpy = GetNextJob();

				OPTICK_TAG("JobValidity", jobCpy.m_EntryPoint == nullptr ? "Invalid" : "Valid");

				if (jobCpy.m_EntryPoint != nullptr)
				{
					// Valid job

					// New job -> Get new fiber!
					// Job will not have a fiber associated with it yet since this case is handled before!
					jobCpy.m_Fiber = GetCurrentFiber();
					jobCpy.m_EntryPoint(jobCpy.m_Param);

					// We might not want to associate a counter with a parallel job!
					if (jobCpy.m_pCounter != nullptr)
					{
						jobCpy.m_pCounter->fetch_sub(1);
					}
					
					jobCpy.m_Fiber = nullptr;
				}
			}
		}

		// Switch back to the initial RunThread Fiber
		SwitchToFiber(thread_fibers.at(GetCurrentThread()));
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

			OPTICK_TAG("FibersLeft", fiber_pool.size());

		}
		return fiber;
	}

	void RunThread()
	{
		OPTICK_THREAD("WORKER");
		LPVOID threadFiber = ConvertThreadToFiber(0);

		// Store the fibers running in this thread inside this list.
		{
			ScopedSpinLock lock(thread_fibers_sl);
			thread_fibers.emplace(GetCurrentThread(), threadFiber);
		}

		SwitchToFiber(GetFiber());

		// Reconvert the fiber to a thread.
		ConvertFiberToThread();
		LOG("Terminated Thread %d", GetCurrentThreadId());
	}

	void InitializeThreadpool(int numOfThreads)
	{
		if (numOfThreads == 0) { return; }
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
			LPVOID fiber = CreateFiber(1024, (LPFIBER_START_ROUTINE) &RunFiber, NULL);
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
		OPTICK_EVENT();
		// Critical section!
		{
			CCE::ScopedSpinLock lock(fiber_pool_sl);
			fiber_pool.push(fiber);
		}
	}

	void KickJob(Job job)
	{
		OPTICK_EVENT();
		OPTICK_TAG("Job", job.m_FunctionName.c_str());
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

	void KickJobs(Job* jobs, int jobCount)
	{
		OPTICK_EVENT();
		OPTICK_TAG("Job", jobs[0].m_FunctionName.c_str());
		ScopedSpinLock lock(job_queue_sl);

		for (int i = 0; i < jobCount; ++i)
		{
			switch (jobs[i].m_Priority)
			{
			case Priority::HIGH:
				job_queue_high.push_back(std::move(jobs[i]));
				break;
			case Priority::NORMAL:
				job_queue_normal.push_back(std::move(jobs[i]));
				break;
			case Priority::LOW:
				job_queue_low.push_back(std::move(jobs[i]));
				break;
			}
		}
	}

	void UpdateWaitData()
	{
		ScopedSpinLock lock(schedule_list_sl);
		if (schedule_list.empty()) 
		{
			return; 
		}

		LPVOID currentFiber = GetCurrentFiber();

		if (schedule_list.contains(currentFiber))
		{
			WaitData waitDataCpy = std::move(schedule_list[currentFiber]);
			schedule_list.erase(currentFiber);

			{
				ScopedSpinLock wl_lock(wait_list_sl);
				wait_list.push_back(std::move(waitDataCpy));
			}
		}
	}

	void BusyWaitForCounter(Counter* const cnt, const int desiredCount)
	{
		OPTICK_EVENT();
		OPTICK_TAG("CurrentCount:", cnt->load(std::memory_order_consume));
		OPTICK_TAG("DesiredCount:", desiredCount);

		if (cnt->load(std::memory_order_consume) > desiredCount)
		{
			// Fetch new fiber
			LPVOID fiber = GetFiber();	
			DASSERT(fiber != nullptr, "Fiber to switch to was null!");
			
			// Schedule for wait list!
			{
				ScopedSpinLock lock(schedule_list_sl);
				schedule_list.emplace(fiber, std::move(WaitData(GetCurrentFiber(), cnt, desiredCount)));
			}

			SwitchToFiber(fiber);
		}
	}

	void BusyWaitForCounterAndFree(Counter* const cnt, const int desiredCount)
	{
		OPTICK_EVENT();
		OPTICK_TAG("CurrentCount:", cnt->load(std::memory_order_consume));
		OPTICK_TAG("DesiredCount:", desiredCount);

		if (cnt->load(std::memory_order_consume) > desiredCount)
		{
			// Fetch new fiber
			LPVOID fiber = GetFiber();
			DASSERT(fiber != nullptr, "Fiber to switch to was null!");
		
			// Schedule for wait list!
			{
				ScopedSpinLock lock(schedule_list_sl);
				schedule_list.emplace(fiber, std::move(WaitData(GetCurrentFiber(), cnt, desiredCount)));
			}

			// Switch to new fiber
			SwitchToFiber(GetFiber());
		}

		delete cnt;
	}
}

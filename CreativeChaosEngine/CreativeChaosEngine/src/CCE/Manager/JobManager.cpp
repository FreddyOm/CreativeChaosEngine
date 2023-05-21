#include "JobManager.h"
#include <winternl.h>
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"
#include "../Utilities/Concurrency/ScopedLock.h"

// TODO: Currently the threads fight for the next job. Fix that to make it more efficient.

namespace CCE
{
	/// <summary>
	/// Starts up the job manager.
	/// </summary>
	void JobManager::StartUp()
	{
		DASSERT(Instance == nullptr, "JobManager was instantiated more than once!");
		JobManager::Instance = this;
		initialized = true;

		auto startTime = Time::CurrentTick();
		PopulateFiberPool();
		SpawnWorkerThreads();

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("JobManager initialized!", COLOR_BLUE);
	}

	/// <summary>
	/// Shuts down the job manager.
	/// </summary>
	void JobManager::ShutDown()
	{
		LOGC("Shutting down JobManager...", COLOR_BLUE);
		initialized = false;

		for (int i = 0; i < fiber_pool.size(); i++)
		{
			DeleteFiber(fiber_pool.front());
			fiber_pool.pop();
		}

		fiber_pool._Get_container().~deque();
		wait_list.clear();

		for (short i = 0; i < worker_threads.size(); i++)
		{
			if(worker_threads.at(i)->joinable())
				worker_threads.at(i)->join();
			worker_threads.at(i)->~thread();
			delete(worker_threads.at(i));
		}
		
		worker_threads.clear();
		Instance = nullptr;
	}

	/// <summary>
	/// Spawns the worker threads.
	/// </summary>
	/// <param name="numOfThreads"></param>
	void JobManager::SpawnWorkerThreads(const short numOfThreads)
	{
#ifdef CCE_PLATFORM_WINDOWS // PLATFORM WINDOWS

		SpawnWorkerThreadsWin(numOfThreads);

#else
#error CCE is currently only supported for Windows
#endif // CCE_PLATFORM_WINDOWS

	}

	/// <summary>
	/// Populate the fiber pool with a given amount of fibers.
	/// </summary>
	/// <param name="numOfFibers">The amount of fibers to spawn. Default is 100.</param>
	void JobManager::PopulateFiberPool(const short numOfFibers)
	{
#ifdef CCE_PLATFORM_WINDOWS // PLATFORM WINDOWS

		PopulateFiberPoolWin(numOfFibers);

#else
#error CCE is currently only supported for Windows
#endif // CCE_PLATFORM_WINDOWS
	}

	/// <summary>
	/// Kicks a job.
	/// </summary>
	/// <param name="decl">Declaration of the job.</param>
	/// <returns>True if job was successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJob(JobManager::JobDeclaration& decl, JobManager::Counter* cnt)
	{
		if (decl.m_pEntryPoint == nullptr) { return false; }

		auto lock = ScopedLock(&jobQueueMutex);
		decl.m_pCounter = cnt;
		
		// Add job to queue depending on its priority
		switch(decl.m_priority)
		{
			case JobManager::Priority::HIGH:
			{
				jobQueue_High.push(std::move(decl)); break;
			}
			case JobManager::Priority::LOW:
			{
				jobQueue_Low.push(std::move(decl)); break;
			}
			default:
			{
				jobQueue_Normal.push(std::move(decl)); break;
			}
		}

		return true;
	}

	//TODO: Implement KickJobAndDeleteDecl to clarify whether or not the decl is still valid afterwards.

	/// <summary>
	/// Kicks a job.
	/// </summary>
	/// <param name="decl">Declaration of the job.</param>
	/// <returns>True if job was successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJob(JobManager::JobDeclaration* decl, JobManager::Counter* cnt)
	{
		if (decl->m_pEntryPoint == nullptr) { return false; }

		auto lock = ScopedLock(&jobQueueMutex);
		decl->m_pCounter = cnt;

		// Add job to queue depending on its priority
		switch (decl->m_priority)
		{
		case JobManager::Priority::HIGH:
		{
			jobQueue_High.push(*decl); break;
		}
		case JobManager::Priority::LOW:
		{
			jobQueue_Low.push(*decl); break;
		}
		default:
		{
			jobQueue_Normal.push(*decl); break;
		}
		}

		return true;
	}

	//TODO: Check how KickJobAndWait should be implemented

	/// <summary>
	/// Waits for another job to be kicked and kicks a job then.
	/// </summary>
	/// <param name="decl">Declaration of the job.</param>
	/// <param name="waitForCnt">The counter to wait for to become 0.</param>
	/// <returns>True if job was successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJobAndWait(JobManager::JobDeclaration& decl, const JobManager::Counter* waitForCnt)
	{
		throw E_NOTIMPL;
		auto lock = ScopedLock(&jobQueueMutex);
		// Add job to queue depending on its priority
		switch (decl.m_priority)
		{
		case JobManager::Priority::HIGH:
		{
			jobQueue_High.push(std::move(decl)); break;
		}
		case JobManager::Priority::LOW:
		{
			jobQueue_Low.push(std::move(decl)); break;
		}
		default:
		{
			jobQueue_Normal.push(std::move(decl)); break;
		}
		}
		DASSERT(waitForCnt != nullptr, "Conter may not be null!");

		return true;
	}

	/// <summary>
	/// Kicks multiple jobs in a row.
	/// </summary>
	/// <param name="count">The amount of jobs to kick.</param>
	/// <param name="decls">The declarations of the jobs.</param>
	/// <returns>True if jobs were successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJobs(int count, JobManager::JobDeclaration decls[], JobManager::Counter* cnt)
	{
		bool success = true;
		for (unsigned short i = 0; i < count; i++)
		{
			if (!KickJob(decls[i], cnt))
			{
				success = false;
			}
		}

		return success;
	}

	//TODO: Check how the counters should be implemented

	/// <summary>
	/// Waits for the counter to become equal to or less than the desired count.
	/// </summary>
	/// <param name="pJobCounter">A pointer to the counter.</param>
	/// <param name="desiredCnt">The desired count for contination.</param>
	void JobManager::WaitForCounter(const Counter* pJobCounter, const int desiredCnt = 0)
	{
		auto now = Time::Now();
		while ((*pJobCounter) > desiredCnt)
		{
			continue;
		}
		auto after = Time::Now();
		//LOG_JOBS("IDLED %i microseconds.", Time::GetDurationInMicroSec(now, after));
	}

	/// <summary>
	/// Waits for the counter to become equal to or less than the desired count.
	/// The counter is freed afterwards.
	/// </summary>
	/// <param name="pJobCounter">A pointer to the counter.</param>
	/// <param name="desiredCnt">The desired count for contination.</param>
	void JobManager::WaitForCounterAndFree(Counter* pJobCounter, const int desiredCnt = 0)
	{
		auto now = Time::CurrentTick();
		while ((*pJobCounter) > desiredCnt)
		{
			continue;
		}
		auto after = Time::CurrentTick();
		LOG_JOBS("IDLED %i ticks.", after - now);
		delete pJobCounter;
	}

	/// <summary>
	/// Spawns multiple worker threads. Windows only version.
	/// </summary>
	/// <param name="numOfThreads">The number of threads to spawn. 
	/// If set to -1, the number will be set to the number of physical cores.</param>
	void JobManager::SpawnWorkerThreadsWin(short numOfThreads)
	{
		if (numOfThreads == -1)
		{
			// Handle default worker threads amount
			numOfThreads = std::thread::hardware_concurrency() - 1;
		}
		
		LOG_JOBS("Number of logical cpu cores: %i", numOfThreads + 1);
		DWORD_PTR processAffinityMask = (DWORD_PTR(1) << (numOfThreads)) - 1;
		
		// Check for errors with process affinity
		bool processAffinityError = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask) == 0;
		if (processAffinityError) { DERROR(GetLastError()); }
		DASSERT(!processAffinityError,"Setting process affinity mask wasn't successful!");

		// Spawn worker threads and set affinity
		for (unsigned short t_index = 0; t_index < numOfThreads; t_index++)
		{
			// Spawn threads		TODO: allocate in custom allocator
			std::thread* workerThread = new std::thread(JobManager::RunThread);
			auto hndl = workerThread->native_handle();

			// Set affinity and hanle error
			bool threadAffinityError = SetThreadAffinityMask(hndl, DWORD_PTR(1) << t_index) == 0;
			if (threadAffinityError) { DERROR(GetLastError()); }
			DASSERT(!threadAffinityError,"Setting thread affinity wasn't successful!");
			
			// Add to list
			worker_threads.push_back(std::move(workerThread));
		}
	}

	/// <summary>
	/// Creates fibers inside the fiber pool. Windows only version.
	/// </summary>
	/// <param name="numOfFibers">The number of fibers to spawn.
	/// Default is 100.</param>
	void JobManager::PopulateFiberPoolWin(const short numOfFibers)
	{
		LOG_JOBS("Creating %i fibers...", numOfFibers);

		// convert main thread to fiber
		mainFiber = ConvertThreadToFiber(NULL);
		DASSERT(mainFiber != nullptr, "Conversion main thread -> fiber not succesful!");

		// populate the fiber pools
		for (int i = 0; i < numOfFibers; i++)
		{
			auto fiber = CreateFiber(
				0,
				(LPFIBER_START_ROUTINE)&JobManager::ExecuteJob,
				NULL);

			DASSERT(fiber != NULL, "Failed creating fiber pool!");
			fiber_pool.push(std::move(fiber));
		}
	}

	/// <summary>
	/// The routine that runs on each thread.
	/// </summary>
	void JobManager::RunThread()
	{
		// convert thread to fiber
		LPVOID _fiber = ConvertThreadToFiber(NULL);

		{
			auto lock = ScopedLock(&threadIdMutex);
			thread_fibers.insert({ GetThreadId(GetCurrentThread()), _fiber });
		}		

		while (JobManager::Instance->initialized)
		{
			if (HasNextJob() || wait_list.size() != 0)
			{
				// Get new fiber and switch context
				LPVOID fiber = GetFiber();
				SwitchToFiber(fiber);

				// Return fiber to pool
				ReturnFiber(fiber);
				continue;
			}
		}
		
		// No jobs left
		LOG_JOBS("Fiber ran out of jobs!");
		DeleteFiber(_fiber);
	}

	// TODO: Implement job wait list functionality so jobs can be put to sleep and woke up later
	/// <summary>
	/// The main execution routine for work.
	/// </summary>
	void JobManager::ExecuteJob()
	{
		// This has to be in a while loop so that the fiber doesn't terminate ever.
		// It seems lik  a terminated fiber will not execute from start again.
		// So in order to not delete and create new fibers all the time we let this run
		// in a loop and make sure the entrance point (SwitchToFiber(GetThreadFiber());) 
		// is at the beginning of the loop!
		while (JobManager::Instance->initialized)
		{
			// Pull the next job
			JOBDECL decl = GetNextJob();

			// This has to be done in here so we don't wake up in the middle of the function
			// when the fiber is pulled next time
			if (decl.m_pEntryPoint != nullptr)
			{
				decl.m_pFiber = GetCurrentFiber();

				// Execute function
				decl.m_pEntryPoint(decl.m_param);

				// Decrease counter after job executed successfully
				if (decl.m_pEntryPoint != nullptr && decl.m_pCounter != nullptr)
				{
					(*decl.m_pCounter)--;
				}
			}

			// In a loop, the end is the beginning!!
			SwitchToFiber(GetThreadFiber());
		}
	}

	/// <summary>
	/// Fetches the fiber that was initially running on the current thread.
	/// </summary>
	/// <returns>A pointer to the fiber.</returns>
	LPVOID JobManager::GetThreadFiber()
	{
		DWORD threadId = GetThreadId(GetCurrentThread());

		auto lock = ScopedLock(&threadIdMutex);
		return thread_fibers.at(threadId);

		DASSERT(false, "The thread was not found!");
		return NULL;
	}

	/// <summary>
	/// Fetches the next job from the job queue.
	/// </summary>
	/// <returns>The next jobs declearation.</returns>
	JobManager::JobDeclaration JobManager::GetNextJob()
	{
		// TODO: Change this to intelligent spin lock (GEA: p. 555)
		// mutex lock for thread safety
		auto lock = CCE::ScopedLock(&jobQueueMutex);

		JobDeclaration decl = {nullptr, Priority::NORMAL};

		if (!jobQueue_High.empty())
		{
			decl = jobQueue_High.front();
			jobQueue_High.pop();

			return decl;
		}

		if (!jobQueue_Normal.empty())
		{
			decl = jobQueue_Normal.front();
			jobQueue_Normal.pop();

			return decl;
		}

		if (!jobQueue_Low.empty())
		{
			decl = jobQueue_Low.front();
			jobQueue_Low.pop();

			return decl;
		}

		// nullptr job decl (check for nullptr / NULL to see if job queues were empty)
		return decl;
	}

	/// <summary>
	/// Returns the status of the job queues.
	/// </summary>
	/// <returns>True if there are any jobs left. False if all job queues are empty.</returns>
	bool JobManager::HasNextJob()
	{
		auto lock = CCE::ScopedLock(&jobQueueMutex);

		return !jobQueue_High.empty() ||
			!jobQueue_Normal.empty() ||
			!jobQueue_Low.empty();
	}

	/// <summary>
	/// Retrives a fiber from the pool. Thread safe.
	/// </summary>
	/// <returns>A fiber from the pool. NULL if empty.</returns>
	LPVOID JobManager::GetFiber()
	{
		if (fiber_pool.empty()) { return NULL; }

		auto lock = CCE::ScopedLock(&fiberMutex);
		LPVOID fiber = fiber_pool.front();
		fiber_pool.pop();

		return fiber;
	}

	/// <summary>
	/// Returns a fiber to the pool. Thread safe.
	/// </summary>
	/// <param name="fiber">The fiber to return.</param>
	void JobManager::ReturnFiber(LPVOID fiber)
	{
		auto lock = ScopedLock(&fiberMutex);
		fiber_pool.push(fiber);
	}

	/// <summary>
	/// Singelton instance of the job manager.
	/// </summary>
	JobManager* JobManager::Instance = nullptr;

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	std::mutex JobManager::jobQueueMutex = std::mutex();

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	std::mutex JobManager::fiberMutex = std::mutex();

	/// <summary>
	/// A mutex lock for pushing the thread id and the fiber;
	/// </summary>
	std::mutex JobManager::threadIdMutex = std::mutex();

	/// <summary>
	/// A pointer to the main fiber.
	/// </summary>
	LPVOID JobManager::mainFiber = nullptr;

	/// <summary>
	/// A collection of fibers to use by the threads.
	/// </summary>
	alignas(8) std::queue<LPVOID> JobManager::fiber_pool;

	/// <summary>
	/// A wait list for jobs to wait on.
	/// </summary>
	alignas(128) std::vector<JobManager::JobDeclaration> JobManager::wait_list;

	/// <summary>
	/// The high priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_High;

	/// <summary>
	/// The normal priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Normal;

	/// <summary>
	/// The low priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Low;

	/// <summary>
	/// A list for th threads to store their fiber handles.
	/// </summary>
	alignas(16) std::unordered_map<DWORD, LPVOID> JobManager::thread_fibers;
}

#include "JobManager.h"
#include <winternl.h>
#include <windows.h>
#include "MemoryManager.h"

// @TODO: Currently the threads fight for the next job. Fix that to make it more efficient.

namespace CCE::Jobs
{
	/// <summary>
	/// Starts up the job manager.
	/// </summary>
	void JobManager::StartUp()
	{
		DASSERT(Instance == nullptr, "JobManager was instantiated more than once!");
		JobManager::Instance = this;
		BaseManager::Init();

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
		BaseManager::Deinit();

		auto lock = ScopedSpinLock(fiberSpinLock);

		// @TODO: Find a better solution (CancellationToken or similar) to 
		// wait for completion of last jobs.
		// Jobs are only runnig while the JobManager is initialized. When it's 
		// being dinitialized, we have to wait until the last jobs finished running
		Sleep(2); // Currently used to make sure, no job is running anymore

		for (int i = 0; i < fiberPool.size(); ++i)
		{
			DeleteFiber(fiberPool.at(i));
		}

		for (short i = 0; i < worker_threads.size(); ++i)
		{
			if(worker_threads.at(i)->joinable())
				worker_threads.at(i)->join();

			worker_threads.at(i)->~thread();
			MemoryManager::Instance->jobMemory.Free<std::thread>(worker_threads.at(i));
			LOG("Joined worker thread!");
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

		auto lock = ScopedSpinLock(jobQueueSpinLock);
		decl.m_pCounter = cnt;

		// Add job to queue depending on its priority
		switch (decl.m_priority)
		{
		case 0:
		{
			jobQueue_High.push(std::move(decl)); break;
		}
		case 1:
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

	// @TODO: Check how KickJobAndWait should be implemented

	/// <summary>
	/// Waits for another job to be kicked and kicks a job then.
	/// </summary>
	/// <param name="decl">Declaration of the job.</param>
	/// <param name="waitForCnt">The counter to wait for to become 0.</param>
	/// <returns>True if job was successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJobAndWait(JobManager::JobDeclaration& decl, const JobManager::Counter* waitForCnt)
	{
		throw E_NOTIMPL;
		auto lock = ScopedSpinLock(jobQueueSpinLock);
		// Add job to queue depending on its priority
		switch (decl.m_priority)
		{
		case 0:
		{
			jobQueue_High.push(std::move(decl)); break;
		}
		case 1:
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

	//TODO: Check how the counters should be implemented

	/// <summary>
	/// Busy waiting and not yielding another job to the thread. 
	/// Only use this on the highest level of job management (main engine loop).
	/// </summary>
	/// <param name="pJobCounter"></param>
	/// <param name="desiredCnt"></param>
	void JobManager::BusyWaitForCounter(Counter& pJobCounter, const int desiredCnt) const
	{
		unsigned short timeout = 127;
		while (pJobCounter.load(std::memory_order_consume) > desiredCnt)
		{
			if (--timeout == 0)
			{
				timeout = 127;
				if (pJobCounter.load(std::memory_order_consume) > desiredCnt)
				{
					int wlp = waitListPointer.load(std::memory_order_consume);
					DASSERT(wlp < NUM_FIBERS - 1,
						"Waitlist is full! Make sure the waitlist pointer is decreased correctly or increase the wait list capacity!");
					
					// Move data to waitlist
					auto lock = ScopedSpinLock(waitListSpinLock);
					waitList.at(waitListPointer.load(std::memory_order_relaxed)) = std::move(WaitData(GetCurrentFiber(),
						&pJobCounter, desiredCnt));
					
					// Atomically increase waitListPointer
					waitListPointer.fetch_add(1, std::memory_order_relaxed);
					
					LPVOID thisThread = GetThreadFiber();
					DASSERT(thisThread != GetCurrentFiber(), "May not switch to the same fíber!");
					SwitchToFiber(thisThread); // Don't go back to this fiber, go back to the fiber that ran on the thread initially!
				}
			}
		}
	}

	/// <summary>
	/// Busy waiting and not yielding another job to the thread. Only use this on the highest level of job management (main engine loop).
	/// </summary>
	/// <param name="pJobCounter"></param>
	/// <param name="desiredCnt"></param>
	void JobManager::BusyWaitForCounterAndFree(Counter& pJobCounter, const int desiredCnt) const
	{
		while (pJobCounter > desiredCnt)
		{
			continue;
		}
		delete &pJobCounter;
	}

	/// <summary>
	/// Spawns multiple worker threads. Windows only version.
	/// </summary>
	/// <param name="numThreads">The number of threads to spawn. 
	/// If set to -1, the number will be set to the number of physical cores - 1.
	/// This makes sure there is one thread left to act as the main thread.</param>
	void JobManager::SpawnWorkerThreadsWin(short numThreads)
	{
		if (numThreads == -1)
		{
			// Handle default worker threads amount
			numThreads = std::thread::hardware_concurrency() - 1;
		}
		
		// Reserve space for the threads
		worker_threads.reserve(numThreads);

		LOG_JOBS("Number of logical cpu cores: %i", numThreads + 1);
		DWORD_PTR processAffinityMask = (DWORD_PTR(1) << (numThreads)) - 1;
		
		// Check for errors with process affinity
		DWORD_PTR processAffinityError = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask);
		if (processAffinityError == 0) { DERROR(GetLastError()); }
		DASSERT(processAffinityError != 0, "Setting process affinity mask wasn't successful!");

		// Spawn worker threads and set affinity
		for (unsigned short t_index = 0; t_index < numThreads; ++t_index)
		{
			// Spawn threads
			std::thread* pWorkerThread = 
				MemoryManager::Instance->jobMemory.Alloc<std::thread>();
			// TODO: Change this so it works without new!!
			new (pWorkerThread)std::thread(JobManager::RunThread);
			auto hndl = pWorkerThread->native_handle();

			// Set affinity and hanle error
			DWORD_PTR threadAffinityResult = SetThreadAffinityMask(hndl, DWORD_PTR(1) << t_index);
			if (threadAffinityResult == 0) { DERROR(GetLastError()); }
			DASSERT(threadAffinityResult != 0,"Setting thread affinity wasn't successful!");
			
			// Add pointer to list
			worker_threads.emplace_back(pWorkerThread);
		}

		LOG_JOBS("Created Thread-Pool containing %i threads.", numThreads);
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
		DASSERT(mainFiber != nullptr, "Conversion 'main thread -> fiber' not succesful!");

		thread_fibers.insert({ GetThreadId(GetCurrentThread()), mainFiber});  // Add main thread to list

		// populate the fiber pools
		for (int i = 0; i < numOfFibers; i++)
		{
			auto fiber = CreateFiber(
				0,
				(LPFIBER_START_ROUTINE)&JobManager::ExecuteJob,
				NULL);

			DASSERT(fiber != NULL, "Failed creating fiber pool!");
			fiberPool.at(i) = std::move(fiber);
		}

		// Top index should be on the last fiber
		fiberPoolPointer = numOfFibers - 1;
	}

	/// <summary>
	/// The routine that runs on each thread.
	/// </summary>
	void JobManager::RunThread()
	{
		// convert thread to fiber
		LOG_JOBS("Converting thread with ID %d to fiber", GetThreadId(GetCurrentThread()));
		LPVOID _fiber = ConvertThreadToFiber(NULL);

		{
			auto lock = ScopedSpinLock(threadIdSpinLock);
			thread_fibers.insert({ GetThreadId(GetCurrentThread()), _fiber });
		}		

		while (JobManager::Instance->IsInitialized())
		{
			if (HasNextJob() || waitListPointer.load(std::memory_order_consume) > 0)
			{
				// Get new fiber and switch context
				LPVOID fiber = GetNextFiber();
				
				if (fiber != NULL)
				{
					SwitchToFiber(fiber);

					// Return fiber to pool if not on wait list
					if(fiber != NULL) 
						ReturnFiber(fiber);
				}
			}
		}
		
		// No jobs left -> end of program
		LOG_JOBS("Fiber terminated due to end of the program!");
		DeleteFiber(_fiber);
	}

	// @TODO: Implement job wait list functionality so jobs can be put to sleep and woke up later
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
		while (JobManager::Instance->IsInitialized())
		{
			// Pull the next job
			JOBDECL decl = GetNextJob();

			// This has to be done in here so we don't wake up in the middle of the function
			// when the fiber is pulled next time
			if (decl.m_pEntryPoint != nullptr)
			{
				DASSERT(decl.m_pFiber == nullptr,
					"Fiber should be nullptr. Jobs that have a non-null fiber should be resolved by the RunThread() function!");

				decl.m_pFiber = GetCurrentFiber();
				
				// Execute function
				decl.m_pEntryPoint(decl.m_param);

				LOG_JOBS("Executed job!");

				// Decrease counter after job executed successfully
				if (decl.m_pEntryPoint != nullptr && decl.m_pCounter != nullptr)
				{
					decl.m_pCounter->fetch_sub(1, std::memory_order_acq_rel);
				}
			}

			// In a loop, the end is the beginning!!  Only switch to new fiber - never to the same
			if(GetThreadFiber() != GetCurrentFiber())
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

		auto lock = ScopedSpinLock(threadIdSpinLock);
		LPVOID fiber = thread_fibers.at(threadId); // @TODO: Can't find main thread in thread_fibers!!

		DASSERT(0 != fiber, "The thread was not found!");
		return fiber;
	}

	/// <summary>
	/// Removes a wait list element at a given index and moves all elements down by one. 
	/// </summary>
	/// <param name="index">The index of the fiber to remove from the wait list.</param>
	/// <returns>Pointer to a fiber.</returns>
	LPVOID JobManager::RemoveWaitListElement(unsigned int index)
	{
		// NO SPINLOCK NEEDED SINCE THIS IS CALLED BY A FUNCTION WHICH ALREADY HOLDS THE LOCK
		LPVOID fiber = waitList.at(index).fiber;

		// Don't swap the last element. This will be redundant but we don't care!
		for (size_t i = index; i < NUM_FIBERS - 1; ++i)
		{
			waitList.at(i) = std::move(waitList.at(i + 1));
		}

		return fiber;
	}

	/// <summary>
	/// Fetches the next job from the job queue.
	/// </summary>
	/// <returns>The next jobs declearation or null if there is none left.</returns>
	JobManager::JobDeclaration JobManager::GetNextJob()
	{
		JobDeclaration decl = { nullptr, 0 };

		auto jobLock = CCE::ScopedSpinLock(jobQueueSpinLock);

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
		auto lock = CCE::ScopedSpinLock(jobQueueSpinLock);

		return !jobQueue_High.empty() ||
			!jobQueue_Normal.empty() ||
			!jobQueue_Low.empty();
	}

	/// <summary>
	/// THREAD SAFE
	/// Retrives a fiber from the pool. Prioritzes waitlist fibers with counters at desired count.
	/// </summary>
	/// <returns>A fiber from the wait list or the pool. NULL if empty.</returns>
	LPVOID JobManager::GetNextFiber()
	{
		if (waitListPointer.load(std::memory_order_consume) > 0)
		{
			auto waitListLock = CCE::ScopedSpinLock(waitListSpinLock);
			for (int i = 0; i <= waitListPointer.load(std::memory_order_acq_rel); ++i)
			{
				if (waitList.at(i).desiredCount <= waitList.at(i).pCounter->load(std::memory_order_acq_rel))
				{
					LPVOID waitListFiber = RemoveWaitListElement(i);
					waitListPointer.fetch_sub(1, std::memory_order_acq_rel);
					return waitListFiber;
				}
			}
		}
		else if(fiberPoolPointer.load(std::memory_order_consume) > 0)
		{
			auto fiberLock = CCE::ScopedSpinLock(fiberSpinLock);
			LPVOID poolfiber = fiberPool.at(fiberPoolPointer.load(std::memory_order_consume));
			fiberPoolPointer.fetch_sub(1, std::memory_order_acq_rel);
			return poolfiber;
		}

		return NULL;
	}

	/// <summary>
	/// Returns a fiber to the pool. Thread safe.
	/// </summary>
	/// <param name="fiber">The fiber to return.</param>
	void JobManager::ReturnFiber(LPVOID fiber)
	{
		auto lock = ScopedSpinLock(fiberSpinLock);
		int fiberPoolIndex = fiberPoolPointer.fetch_add(1, std::memory_order_relaxed);
		fiberPool.at(fiberPoolIndex) = std::move(fiber);
	}

	/// <summary>
	/// Singelton instance of the job manager.
	/// </summary>
	JobManager* JobManager::Instance = nullptr;

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	SpinLock JobManager::jobQueueSpinLock = SpinLock();

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	SpinLock JobManager::fiberSpinLock = SpinLock();

	/// <summary>
	/// A mutex lock for pushing the thread id and the fiber;
	/// </summary>
	SpinLock JobManager::threadIdSpinLock = SpinLock();

	/// <summary>
	/// A mutex lock for pushing and pulling from/to the waitList;
	/// </summary>
	SpinLock JobManager::waitListSpinLock = SpinLock();

	/// <summary>
	/// A pointer to the main fiber.
	/// </summary>
	LPVOID JobManager::mainFiber = nullptr;

	/// <summary>
	/// A collection of fibers to use by the threads.
	/// </summary>
	alignas(8) std::array<LPVOID, NUM_FIBERS> JobManager::fiberPool = {};

	/// <summary>
	/// A wait list for jobs to wait on.
	/// </summary>
	alignas(128) std::array<JobManager::WaitData, NUM_FIBERS> JobManager::waitList = {};

	/// <summary>
	/// The pointer to the top most fiber in the waitlist
	/// </summary>
	std::atomic<int> JobManager::waitListPointer = 0;

	/// <summary>
	/// The high priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_High = {};

	/// <summary>
	/// The normal priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Normal = {};

	/// <summary>
	/// The low priority queue for jobs.
	/// </summary>
	alignas(128) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Low = {};

	/// <summary>
	/// A list for th threads to store their fiber handles.
	/// </summary>
	alignas(16) std::unordered_map<DWORD, LPVOID> JobManager::thread_fibers = {};

	/// <summary>
	/// A pointer to the topmost fiber in the fiber pool.
	/// </summary>
	std::atomic<unsigned int> JobManager::fiberPoolPointer = 0;
}

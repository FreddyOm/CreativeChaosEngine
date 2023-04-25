#include "JobManager.h"
#include <winternl.h>
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"
#include "../Utilities/Concurrency/ScopedLock.h"

namespace CCE
{
	/// <summary>
	/// Starts up the job manager.
	/// </summary>
	void JobManager::StartUp()
	{
		static JobManager* Instance;
		DASSERT(Instance == nullptr, "JobManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		PopulateFiberPool();
		//SpawnWorkerThreads();
		initialized = true;

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
	bool JobManager::KickJob(const JobManager::JobDeclaration& decl, const JobManager::Counter* cnt)
	{
		auto lock = ScopedLock(&kickJobMutex);
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
		if(cnt != nullptr)
			*cnt++;

		return true;
	}

	/// <summary>
	/// Kicks multiple jobs in a row.
	/// </summary>
	/// <param name="count">The amount of jobs to kick.</param>
	/// <param name="decls">The declarations of the jobs.</param>
	/// <returns>True if jobs were successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJobs(int count, const JobManager::JobDeclaration decls[], const JobManager::Counter* cnt)
	{
		auto lock = ScopedLock(&kickJobsMutex);
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

	/// <summary>
	/// Waits for the counter to become equal to or less than the desired count.
	/// </summary>
	/// <param name="pJobCounter">A pointer to the counter.</param>
	/// <param name="desiredCnt">The desired count for contination.</param>
	void JobManager::WaitForCounter(const Counter* pJobCounter, const int desiredCnt = 0)
	{
		while (pJobCounter->counter > desiredCnt) continue;
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
			// handle default worker threads amount
			numOfThreads = std::thread::hardware_concurrency();
		}
		
		LOG_JOBS("Number of logical cpu cores: %i", numOfThreads);
		DWORD_PTR processAffinityMask = (DWORD_PTR(1) << (numOfThreads)) - 1;
		
		// check for errors with process affinity
		bool processAffinityError = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask) == 0;
		if (processAffinityError) { DERROR(GetLastError()); }
		DASSERT(!processAffinityError,"Setting process affinity mask wasn't successful!");

		// spawn worker threads and set affinity
		for (unsigned short t_index = 0; t_index < numOfThreads; t_index++)
		{
			// spawn threads
			std::thread* workerThread = new std::thread(JobManager::RunThread);
			auto hndl = workerThread->native_handle();

			// set affinity and hanle error
			bool threadAffinityError = SetThreadAffinityMask(hndl, DWORD_PTR(1) << t_index) == 0;
			if (threadAffinityError) { DERROR(GetLastError()); }
			DASSERT(!threadAffinityError,"Setting thread affinity wasn't successful!");
			
			// add to list
			worker_threads.push_back(workerThread);
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
			fiber_pool.push(fiber);
		}
	}

	// TODO: Pull jobs and work on them
	// TODO: Fix job pulling and working on JobDecls
	/// <summary>
	/// Do some work for now
	/// </summary>
	void JobManager::RunThread()
	{
		// convert thread to fiber
		LPVOID _fiber = ConvertThreadToFiber(NULL);
		std::pair<DWORD, LPVOID> thrd_fbr = std::make_pair(GetThreadId(GetCurrentThread()), _fiber);

		{
			auto lock = ScopedLock(&pushThreadIdMutex);
			thread_fibers.push_back(thrd_fbr);
		}		

		while (HasNextJob())
		{
			if (HasNextJob() || wait_list.size() != 0)
			{
				// Get new fiber and switch context
				LPVOID fiber = GetFiber();
				SwitchToFiber(fiber);
				
				// Finished executing job
				LOG_JOBS("Executed job");
				
				// Return fiber to pool
				ReturnFiber(fiber);
			}
		}
		
		// No jobs left
		LOG_JOBS("Fiber ran out of jobs!");
		DeleteFiber(_fiber);
	}

	/// <summary>
	/// The main execution routine for work.
	/// </summary>
	void JobManager::ExecuteJob()
	{
		// Pull the next job
		JOBDECL decl = GetNextJob();
		DASSERT(decl.m_pEntryPoint != nullptr,
			"The jobs decleration is invalid!\n This is probably due to a race condition.");
		decl.m_pFiber = GetCurrentFiber();

		// Execute function
		decl.m_pEntryPoint(decl.m_param);

		// Exit the routine
		// Hier kommt die Rückführung hin
		SwitchToFiber(GetThreadFiber());
	}

	/// <summary>
	/// Fetches the fiber that was initially running on the current thread.
	/// </summary>
	/// <returns>A pointer to the fiber.</returns>
	LPVOID JobManager::GetThreadFiber()
	{
		DWORD threadId = GetThreadId(GetCurrentThread());

		auto lock = ScopedLock(&pushThreadIdMutex);

		// TODO: Make this more performant by using hashed values
		for (size_t i = 0; i < thread_fibers.size(); i++)
		{
			if (thread_fibers[i].first == threadId)
			{
				return thread_fibers[i].second;
			}
		}

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
		auto lock = CCE::ScopedLock(&getJobMutex);
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
		auto lock = CCE::ScopedLock(&hasNextMutex);

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

		auto lock = CCE::ScopedLock(&getFiberMutex);
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
		// TODO: Reset the fibers context if possible using 
		// SetThreadContext and CONTEXT or in any other way
		auto lock = ScopedLock(&returnFiberMutex);
		fiber_pool.push(fiber);
	}

	/// <summary>
	/// Singelton instance of the job manager.
	/// </summary>
	JobManager* JobManager::Instance = nullptr;

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	std::mutex JobManager::getJobMutex = std::mutex();
	
	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	std::mutex JobManager::getFiberMutex = std::mutex();

	/// <summary>
	/// A mutex lock for getting jobInformation;
	/// </summary>
	std::mutex JobManager::returnFiberMutex = std::mutex();

	/// <summary>
	/// A mutex lock for kicking a job;
	/// </summary>
	std::mutex JobManager::kickJobMutex = std::mutex();

	/// <summary>
	/// A mutex lock for kicking jobs;
	/// </summary>
	std::mutex JobManager::kickJobsMutex = std::mutex();

	/// <summary>
	/// A mutex lock for checking if jobs are left;
	/// </summary>
	std::mutex JobManager::hasNextMutex = std::mutex();

	/// <summary>
	/// A mutex lock for pushing the thread id and the fiber;
	/// </summary>
	std::mutex JobManager::pushThreadIdMutex = std::mutex();

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
	alignas(32) std::vector<std::pair<JobManager::JobDeclaration, LPVOID>> JobManager::wait_list;

	/// <summary>
	/// The high priority queue for jobs.
	/// </summary>
	alignas(32) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_High;

	/// <summary>
	/// The normal priority queue for jobs.
	/// </summary>
	alignas(32) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Normal;

	/// <summary>
	/// The low priority queue for jobs.
	/// </summary>
	alignas(32) std::queue<JobManager::JobDeclaration> JobManager::jobQueue_Low;

	/// <summary>
	/// A list for th threads to store their fiber handles.
	/// </summary>
	std::vector<std::pair<DWORD, LPVOID>> JobManager::thread_fibers;
}

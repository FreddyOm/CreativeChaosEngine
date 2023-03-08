#include "JobManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

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
		SpawnWorkerThreads();
		PopulateFiberPool();
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
		for (short i = 0; i < worker_threads.size(); i++)
		{
			if(worker_threads.at(i)->joinable())
				worker_threads.at(i)->join();
			worker_threads.at(i)->~thread();
			delete(worker_threads.at(i));
		}

		worker_threads.clear();
		initialized = false;
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
	bool JobManager::KickJob(const Job::Declaration& decl)
	{
		Job _job = Job(decl);
		switch(decl.m_priority)
		{
		case Job::Priority::HIGH:
		{
			jobQueue_High.push(_job);
			break;
		}
		case Job::Priority::NORMAL:
		{
			jobQueue_Normal.push(_job);
			break;
		}
		default:
		{
			jobQueue_Low.push(_job);
			break;
		}
		}

		return true;
	}

	/// <summary>
	/// Kicks multiple jobs in a row.
	/// </summary>
	/// <param name="count">The amount of jobs to kick.</param>
	/// <param name="decls">The declarations of the jobs.</param>
	/// <returns>True if jobs were successfully kicked, false if an error occured.</returns>
	bool JobManager::KickJobs(int count, const JobManager::Job::Declaration decls[])
	{
		bool success = true;
		for (unsigned short i = 0; i < count; i++)
		{
			if (!KickJob(decls[i]))
			{
				success = false;
			}
		}

		return success;
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

		DWORD_PTR processAffinityMask = (DWORD_PTR(1) << numOfThreads) - 1;
		LOG("Process Affinity Mask: %u", processAffinityMask); // FIX ME: Remove this later

		// check for errors with process affinity
		bool processAffinityError = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask) == 0;
		if (processAffinityError) { DERROR(GetLastError()); }
		DASSERT(!processAffinityError,"Setting process affinity mask wasn't successful!");

		// convert main thread to fiber
		mainFiber = ConvertThreadToFiber(NULL);
		DASSERT(mainFiber != nullptr, "Conversion main thread -> fiber not succesful!");

		// spawn worker threads and set affinity
		for (unsigned short t_index = 0; t_index < numOfThreads; t_index++)
		{
			// spawn threads
			std::thread* workerThread = new std::thread(JobManager::RunThread);
			auto hndl = workerThread->native_handle();

			// set affinity and hanle error
			LOG("Parameter %i", DWORD_PTR(1) << t_index);

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
		// populate the fiber pools
		for (int i = 0; i < numOfFibers; i++)
		{
			fiber_pool.push_back(Fiber(i, Fiber::FiberContext()));
		}
	}

	// TODO: Pull jobs and work on them
	/// <summary>
	/// Do some work for now
	/// </summary>
	void JobManager::RunThread()
	{
		// convert thread to fiver
		ConvertThreadToFiber(NULL);
		DASSERT(IsThreadAFiber(), "Thread could not be converted to fiber.");
		
		// do stuff
		LOG_JOBS("Doing work...");
	}

	/// <summary>
	/// Singelton instance of the job manager.
	/// </summary>
	JobManager* JobManager::Instance = nullptr;

	/// <summary>
	/// Static index for the jobs.
	/// </summary>
	unsigned int JobManager::Job::g_index = 0;
}
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
		Instance = nullptr;
	}

	/// <summary>
	/// Spawns the worker threads.
	/// </summary>
	/// <param name="numOfThreads"></param>
	void JobManager::SpawnWorkerThreads(short numOfThreads)
	{
#ifdef CCE_PLATFORM_WINDOWS // PLATFORM WINDOWS

		SpawnWorkerThreadsWin(numOfThreads);

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
		
		LOG_JOBS("Number of physical cpu cores: %i", numOfThreads);

		mainFiber = ConvertThreadToFiber(NULL);
		DASSERT(mainFiber != nullptr, "Conversion main thread -> fiber not succesful!");
		
		//worker_threads = std::vector<std::thread>(numOfThreads);

		for (unsigned short t_index = 0; t_index < numOfThreads; t_index++)
		{
			// spawn threads with cpu affinity

		}

		/*
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(i, &cpuset);
		int rc = pthread_setaffinity_np(threads[i].native_handle(),
			sizeof(cpu_set_t), &cpuset);
			*/
	}

	/// <summary>
	/// Singelton instance of the job manager.
	/// </summary>
	JobManager* JobManager::Instance = nullptr;

	unsigned int JobManager::Job::g_index = 0;
}
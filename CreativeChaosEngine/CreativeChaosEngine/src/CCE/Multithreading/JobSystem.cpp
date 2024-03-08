#include "JobSystem.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"

namespace CCE::Jobs
{
	std::atomic<bool> runThreads(true);
	std::vector<std::thread*> worker_threads = {};

	void InitializeThreadpool(int numOfThreads)
	{
		// Define number of threads
		if(numOfThreads < 1)
			numOfThreads = std::thread::hardware_concurrency() - 1;
		
		LOG_JOBS("Number of logical cpu cores: %i", numOfThreads + 1);
		worker_threads.reserve(numOfThreads);

		// Set affinity mask
		DWORD_PTR processAffinityMask = (DWORD_PTR(1) << (numOfThreads)) - 1;

		// Check for errors with process affinity
		bool processAffinityError = SetProcessAffinityMask(GetCurrentProcess(), processAffinityMask) == 0;
		if (processAffinityError) { DERROR(GetLastError()); }
		DASSERT(!processAffinityError, "Setting process affinity mask wasn't successful!");

		std::thread* workerThread = nullptr;
		runThreads.store(true, std::memory_order_relaxed);

		// Create worker threads
		for (unsigned short t_index = 0; t_index < numOfThreads; ++t_index)
		{
			// Spawn threads
			workerThread = new std::thread(RunThread);
			auto hndl = workerThread->native_handle();

			// Set affinity and hanle error
			bool threadAffinityError = SetThreadAffinityMask(hndl, DWORD_PTR(1) << t_index) == 0;
			if (threadAffinityError) { DERROR(GetLastError()); }
			DASSERT(!threadAffinityError, "Setting thread affinity wasn't successful!");

			// Add to list
			worker_threads.push_back(workerThread);
		}

	}

	void DeinitializeThreadpool()
	{
		runThreads.store(false, std::memory_order_acquire);

		Sleep(1);
		
		for (std::thread* workerThread : worker_threads)
		{
			if (workerThread->joinable())
				workerThread->join();

			delete workerThread;
		}
	}

	void RunThread()
	{
		while (runThreads.load(std::memory_order_consume))
		{
			// Do stuff here!
			LOG("Hello I am a thread!");
		}
	}
}

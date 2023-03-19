#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>
#include <emmintrin.h>
#include <winnt.h>
#include <mutex>
#include "../String/String.h"
#include "../Memory/PoolAllocator.h"
#include "../Analysis/Logger.h"

namespace CCE
{
#define NUM_FIBERS 100
#define SIZE_FIBER_CNTXT 65776
#define JOBDECL CCE::JobManager::JobDeclaration

	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static JobManager* Instance;		

		typedef void EntryPoint(va_list param);

		enum class Priority
		{
			HIGH,
			NORMAL,
			LOW
		};
		struct Counter
		{

		};
		struct Fiber
		{

		};
		struct JobDeclaration // 64 bytes
		{
			CCE::String m_Description = "Job";		// 8 bytes
			EntryPoint* m_pEntryPoint = nullptr;	// 8 bytes

			va_list m_param = NULL;					// 8 bytes
			Counter* m_pCounter = nullptr;			// 8 bytes

			Priority m_priority = Priority::NORMAL;	// 4 bytes
			byte padding[28] = {};					// 28 bytes

			JobDeclaration() = default;
			JobDeclaration(void* ep, Priority pr, ...)
			{
				m_Description = "Job";
				m_pEntryPoint = (EntryPoint*)ep;

				va_list args;
				va_start(args, pr);
				va_end(args);

				m_param = args;
				m_pCounter = nullptr;
				m_priority = pr;
			}
		};

		void SpawnWorkerThreads(const short numOfThreads = -1);
		void PopulateFiberPool(const short numOfFibers = NUM_FIBERS);
		bool KickJob(const JobDeclaration& decl);
		bool KickJobs(int count, const JobDeclaration decls[]);

	private:
		void SpawnWorkerThreadsWin(const short numOfThreads = -1);
		void PopulateFiberPoolWin(const short numOfFibers);
		
		static void RunThread();
		static JobDeclaration GetNextJob();
		static bool HasNextJob();
		static std::mutex getJobMutex;
		static std::mutex kickJobMutex;

	private:
		LPVOID mainFiber = nullptr;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
		alignas(8) static std::queue<Fiber> fiber_pool;
		alignas(64) static std::vector<std::pair<JobDeclaration, Fiber>> wait_list;
		
		// TODO: Implement custom queue class
		alignas(64) static std::queue<JobDeclaration> jobQueue_High;
		alignas(64) static std::queue<JobDeclaration> jobQueue_Normal;
		alignas(64) static std::queue<JobDeclaration> jobQueue_Low;

		alignas(256) CCMemory::PoolAllocator fiberContextPool =
			CCMemory::PoolAllocator(NUM_FIBERS, SIZE_FIBER_CNTXT);
	};
}

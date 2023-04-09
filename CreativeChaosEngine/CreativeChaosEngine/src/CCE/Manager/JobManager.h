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
#include "../Utilities/Events/Delegate.h"

namespace CCE
{
#define NUM_FIBERS 100
#define SIZE_FIBER_CNTXT 65776
#define JOBDECL CCE::JobManager::JobDeclaration
#define BIND(func, obj, ...) std::bind(&func, obj, ##__VA_ARGS__)

using namespace Events;

	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static JobManager* Instance;		

		typedef std::function<void(va_list)> EntryPoint;

		enum class Priority
		{
			HIGH,
			NORMAL,
			LOW
		};
		struct Counter	// 4 bytes
		{
			unsigned int counter = 0;		// 4 bytes
		};
		struct Fiber
		{

		};

		struct JobDeclaration // 32 bytes
		{
			CCE::String m_Description = "Job";		// 8 bytes
			EntryPoint m_pEntryPoint;	// 8 bytes

			va_list m_param = NULL;					// 8 bytes
			Priority m_priority = Priority::NORMAL;	// 4 bytes
			byte padding[4] = {};					// 4 bytes

			JobDeclaration() = default;
			JobDeclaration(const EntryPoint& ep, Priority pr, ...)
			{
				m_Description = "Job";
				m_pEntryPoint = ep;

				va_list args;
				va_start(args, pr);
				va_end(args);

				m_param = args;
				m_priority = pr;
			}
		};

		bool KickJob(const JobDeclaration& decl, const Counter* cnt = nullptr);
		bool KickJobs(int count, const JobDeclaration decls[], const Counter* pJobCounter = nullptr);
		void WaitForCounter(const Counter* pJobCounter, const int desiredCnt);
	
	private:
		void SpawnWorkerThreadsWin(const short numOfThreads = -1);
		void PopulateFiberPoolWin(const short numOfFibers);
		void SpawnWorkerThreads(const short numOfThreads = -1);
		void PopulateFiberPool(const short numOfFibers = NUM_FIBERS);
		
		static void RunThread();
		static JobDeclaration GetNextJob();
		static bool HasNextJob();
		static std::mutex getJobMutex;
		static std::mutex kickJobMutex;

	private:
		static LPVOID mainFiber;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
		alignas(8) static std::queue<LPVOID> fiber_pool;
		alignas(32) static std::vector<std::pair<JobDeclaration, Fiber>> wait_list;
		
		// TODO: Implement custom queue class
		alignas(32) static std::queue<JobDeclaration> jobQueue_High;
		alignas(32) static std::queue<JobDeclaration> jobQueue_Normal;
		alignas(32) static std::queue<JobDeclaration> jobQueue_Low;

		alignas(256) CCMemory::PoolAllocator fiberContextPool =
			CCMemory::PoolAllocator(NUM_FIBERS, SIZE_FIBER_CNTXT);
	};
}

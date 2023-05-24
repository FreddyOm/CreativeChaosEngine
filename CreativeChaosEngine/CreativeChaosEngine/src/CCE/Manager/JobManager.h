#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>
#include <emmintrin.h>
#include <winnt.h>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include "../String/String.h"
#include "../Memory/PoolAllocator.h"
#include "../Analysis/Logger.h"
#include "../Utilities/Events/Delegate.h"

namespace CCE
{
#define NUM_FIBERS 10
#define MAX_JOBS 500
#define WAIT_CNTR_LOOPS 200
#define JOBDECL CCE::JobManager::JobDeclaration
	// TODO: Override an operator to make the binding even more easy to use
#define BIND(func, obj, ...) std::bind(&func, obj, ##__VA_ARGS__)
#define BIND(func, ...) [&](...){return func(##__VA_ARGS__);};
#define JOB_ENTRY_POINT void

using namespace Events;

	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		typedef std::function<JOB_ENTRY_POINT(va_list)> EntryPoint;
		typedef std::atomic<unsigned int> Counter;

		static JobManager* Instance;

		enum class alignas(4) Priority
		{
			HIGH,
			NORMAL,
			LOW
		};

		struct alignas(128) JobDeclaration // 128 bytes
		{
			EntryPoint m_pEntryPoint;				// 64 bytes

			CCE::String m_Description = "Job";		// 8 bytes
			LPVOID m_pFiber = NULL;					// 8 bytes

			Counter* m_pCounter = NULL;				// 8 bytes
			Priority m_priority = Priority::NORMAL;	// 4 bytes
			unsigned int mDesiredCount = 0;			// 4 bytes

			va_list m_param = NULL;					// 8 bytes
			byte padding[20];						// 20 bytes

			JobDeclaration()
			{
				m_pEntryPoint = nullptr;
			}

			JobDeclaration(const EntryPoint& ep, Priority pr, ...)
			{
				m_pEntryPoint = ep;
				m_priority = pr;

				va_list args;
				va_start(args, pr);
				va_end(args);

				m_param = args;
			}
		};

		bool KickJobAndFreeDecl(JobDeclaration& decl, Counter* cnt = nullptr);
		bool KickJob(JobDeclaration* decl, Counter* cnt = nullptr);
		bool KickJobAndWait(JobDeclaration& decl, const Counter* waitForCnt);
		bool KickJobs(int count, JobDeclaration decls[], Counter* pJobCounter = nullptr);
		void WaitForCounter(const Counter* pJobCounter, const int desiredCnt);
		void WaitForCounterAndFree(Counter* pJobCounter, const int desiredCnt);
		void SpawnWorkerThreads(const short numOfThreads = -1);

	private:

		void SpawnWorkerThreadsWin(const short numOfThreads = -1);
		void PopulateFiberPoolWin(const short numOfFibers);
		void PopulateFiberPool(const short numOfFibers = NUM_FIBERS);
		
		static LPVOID GetFiber();
		static void ReturnFiber(LPVOID fiber);

		static void RunThread();
		static void ExecuteJob();
		static JobDeclaration GetNextJob();
		static bool HasNextJob();
		static LPVOID GetThreadFiber();

		// TODO: Fix problem with accessing resources using different mutexes
		static std::mutex jobQueueMutex;
		static std::mutex fiberMutex;
		static std::mutex threadIdMutex;

	private:

		// Wait list for Fibers and their jobs
		alignas(128) static std::vector<JobDeclaration> wait_list;
		
		// TODO: Implement custom queue class
		// TODO: Preallocate -> use pool alloc
		alignas(128) static std::queue<JobDeclaration> jobQueue_High;
		alignas(128) static std::queue<JobDeclaration> jobQueue_Normal;
		alignas(128) static std::queue<JobDeclaration> jobQueue_Low;

		static LPVOID mainFiber;	// 8 bytes
		alignas(8) static std::queue<LPVOID> fiber_pool;
		alignas(16) static std::unordered_map<DWORD, LPVOID> thread_fibers;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
	};
}

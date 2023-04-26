#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>
#include <emmintrin.h>
#include <winnt.h>
#include <mutex>
#include <atomic>
#include "../String/String.h"
#include "../Memory/PoolAllocator.h"
#include "../Analysis/Logger.h"
#include "../Utilities/Events/Delegate.h"

namespace CCE
{
#define NUM_FIBERS 100
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
		typedef std::atomic<unsigned int> Counter;

		enum class Priority
		{
			HIGH,
			NORMAL,
			LOW
		};

		struct JobDeclaration // 32 bytes
		{
			CCE::String m_Description = "Job";		// 8 bytes
			EntryPoint m_pEntryPoint;				// 8 bytes

			va_list m_param = NULL;					// 8 bytes
			Priority m_priority = Priority::NORMAL;	// 4 bytes
			LPVOID m_pFiber = NULL;					// 4 bytes

			Counter* m_pCounter = NULL;				// 4 bytes

			JobDeclaration() = default;
			JobDeclaration(const EntryPoint& ep, Priority pr, ...)
			{
				m_Description = CCE::String(__FUNCTION__);
				m_pEntryPoint = ep;

				va_list args;
				va_start(args, pr);
				va_end(args);

				m_param = args;
				m_priority = pr;
			}
		};

		bool KickJob(JobDeclaration& decl, Counter* cnt = nullptr);
		bool WaitAndKickJob(const JobDeclaration& decl, const Counter* waitForCnt);
		bool KickJobs(int count, JobDeclaration decls[], Counter* pJobCounter = nullptr);
		void WaitForCounter(const Counter* pJobCounter, const int desiredCnt);
		void WaitForCounterAndFree(const Counter* pJobCounter, const int desiredCnt);
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

		static std::mutex getJobMutex;
		static std::mutex getFiberMutex;
		static std::mutex returnFiberMutex;
		static std::mutex kickJobMutex;
		static std::mutex kickJobsMutex;
		static std::mutex hasNextMutex;
		static std::mutex pushThreadIdMutex;

	//private:
	public:
		// Wait list for Fibers and their jobs
		alignas(32) static std::vector<std::pair<JobDeclaration, LPVOID>> wait_list;
		
		// TODO: Implement custom queue class
		alignas(32) static std::queue<JobDeclaration> jobQueue_High;
		alignas(32) static std::queue<JobDeclaration> jobQueue_Normal;
		alignas(32) static std::queue<JobDeclaration> jobQueue_Low;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
		alignas(8) static std::queue<LPVOID> fiber_pool;
		static std::vector<std::pair<DWORD, LPVOID>> thread_fibers;

		static LPVOID mainFiber;
	};
}

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
#define JOBDECL CCE::JobManager::Job::Declaration

	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static JobManager* Instance;		

//	protected:
		struct Job // 64 bytes
		{
		public:
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
			struct Declaration // 48 bytes
			{
				CCE::String m_Description = "Job";		// 8 bytes
				EntryPoint* m_pEntryPoint = nullptr;	// 8 bytes

				va_list m_param = NULL;					// 8 bytes
				Counter* m_pCounter = nullptr;			// 8 bytes
				
				Priority m_priority = Priority::NORMAL;	// 4 bytes
				byte padding[12] = {};					// 12 bytes

				Declaration() = default;
				Declaration(void* ep, Priority pr, ...)
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

			Job() = default;
			Job(Declaration decl)
			{
				id = g_index++;
				m_Declaration = decl;
			}
			
		public:
			static void ResetIdIndex();

		public:
			Declaration m_Declaration = {};				// 48 bytes

			static unsigned int g_index;				// 4 bytes
			unsigned int id = 0;						// 4 bytes			
			byte padding[8] = {};						// 8 bytes
		};

		struct CCE_API Fiber // 8 bytes
		{
			struct FiberContext // 65776 byte
			{
				NT_TIB64 stackInfo;			// stackInfo
				byte fls[65536];			// fiber locas stack
			};

			Fiber() = default;

			Fiber(unsigned int _id, FiberContext* _cntxt)
			{
				id = _id;
				cntxt = _cntxt;
			}
			~Fiber()
			{
				id = 0;
				currentJob = nullptr;
				cntxt = nullptr;
			}

			unsigned int id = 0;						// 4 bytes
			Job* currentJob = nullptr;					// 4 bytes ?
			FiberContext* cntxt = nullptr;				// 1 byte
		};
		void SpawnWorkerThreads(const short numOfThreads = -1);
		void PopulateFiberPool(const short numOfFibers = NUM_FIBERS);
		bool KickJob(const Job::Declaration& decl);
		bool KickJobs(int count, const Job::Declaration decls[]);

	private:
		void SpawnWorkerThreadsWin(const short numOfThreads = -1);
		void PopulateFiberPoolWin(const short numOfFibers);
		
		static void RunThread();
		static Job GetNextJob();
		static bool HasNextJob();
		static std::mutex getJobMutex;
		static std::mutex kickJobMutex;

	private:
		LPVOID mainFiber = nullptr;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
		alignas(8) static std::queue<Fiber> fiber_pool;
		alignas(64) static std::vector<std::pair<Job, Fiber>> wait_list;
		
		// TODO: Implement custom queue class
		alignas(64) static std::queue<Job> jobQueue_High;
		alignas(64) static std::queue<Job> jobQueue_Normal;
		alignas(64) static std::queue<Job> jobQueue_Low;

		alignas(256) CCMemory::PoolAllocator fiberContextPool =
			CCMemory::PoolAllocator(NUM_FIBERS, SIZE_FIBER_CNTXT);
	};
}

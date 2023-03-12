#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>
#include "../String/String.h"
#include "../Memory/PoolAllocator.h"
#include <emmintrin.h>

namespace CCE
{
#define NUM_FIBERS 100
#define SIZE_FIBER_CNTXT 65776

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
			typedef void EntryPoint(uintptr_t param);
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
				CCE::String m_Description;	// 8 bytes
				EntryPoint* m_pEntryPoint;	// 8 bytes

				va_list m_param;			// 8 bytes
				Counter* m_pCounter;		// 8 bytes
				
				Priority m_priority;		// 4 bytes
				byte padding[12];			// 12 bytes
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

		struct Fiber // 8 bytes
		{
			struct FiberContext // 65776 byte
			{
				char stack[65536];					// 65536 bytes
				void* rip, * rsp;					// 8 bytes
				void* rbx, * rbp, * r12, * r13, * r14, * r15, * rdi, * rsi;
				__m128i xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15;
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
		
	private:
		LPVOID mainFiber = nullptr;

		// TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads;
		alignas(8) std::vector<Fiber> fiber_pool;
		alignas(64) std::vector<Job> wait_list;
		
		// TODO: Implement custom queue class
		alignas(64) std::queue<Job> jobQueue_High;
		alignas(64) std::queue<Job> jobQueue_Normal;
		alignas(64) std::queue<Job> jobQueue_Low;

		alignas(256) CCMemory::PoolAllocator fiberContextPool =
			CCMemory::PoolAllocator(NUM_FIBERS, SIZE_FIBER_CNTXT);
	};
}

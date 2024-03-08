#pragma once
#include "BaseManager.h"
#include "../Multithreading/ScopedSpinLock.h"
#include "../Multithreading/SpinLock.h"
#include "../Memory/PoolAllocator.h"
#include "../Analysis/Logger.h"
#include "../String/String.h"
#include <unordered_map>
#include <emmintrin.h>
#include <winternl.h>
#include <functional>
#include <winnt.h>
#include <thread>
#include <vector>
#include <atomic>
#include <queue>
#include <mutex>
#include <array>

namespace CCE
{
#define NUM_FIBERS (unsigned int)100
#define MAX_JOBS 500
#define WAIT_CNTR_LOOPS 200
#define JOBDECL CCE::JobManager::JobDeclaration
	// @TODO: Override an operator to make the binding even more easy to use
#define BIND_BASIC(func, obj, ...) std::bind(&func, obj, ##__VA_ARGS__)
#define BIND(func, ...) [&](...){return func(##__VA_ARGS__);};
#define JOB_ENTRY_POINT void

	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		typedef std::function<JOB_ENTRY_POINT(va_list)> EntryPoint;
		typedef std::atomic<int> Counter;

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

			LPVOID m_pFiber = NULL;					// 8 bytes
			Counter* m_pCounter = NULL;				// 8 bytes

			Priority m_priority = Priority::NORMAL;	// 4 bytes
			unsigned int mDesiredCount = 0;			// 4 bytes
			va_list m_param = NULL;					// 8 bytes

			byte padding[32] = {};					// 32 bytes

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

			JobDeclaration(const JobDeclaration& other)
			{
				m_pEntryPoint = other.m_pEntryPoint;
				m_pFiber = other.m_pFiber;
				m_pCounter = other.m_pCounter;
				m_priority = other.m_priority;
				mDesiredCount = other.mDesiredCount;
				m_param = other.m_param;
			}

			JobDeclaration(JobDeclaration&& other) noexcept
			{
				m_pEntryPoint = other.m_pEntryPoint;
				m_pFiber = other.m_pFiber;
				m_pCounter = other.m_pCounter;
				m_priority = other.m_priority;
				mDesiredCount = other.mDesiredCount;
				m_param = other.m_param;
			}

			JobDeclaration& operator=(const JobDeclaration& other)
			{
				m_pEntryPoint = other.m_pEntryPoint;
				m_pFiber = other.m_pFiber;
				m_pCounter = other.m_pCounter;
				m_priority = other.m_priority;
				mDesiredCount = other.mDesiredCount;
				m_param = other.m_param;

				return *this;
			}

			JobDeclaration& operator=(JobDeclaration&& other) noexcept
			{
				m_pEntryPoint = other.m_pEntryPoint;
				m_pFiber = other.m_pFiber;
				m_pCounter = other.m_pCounter;
				m_priority = other.m_priority;
				mDesiredCount = other.mDesiredCount;
				m_param = other.m_param;

				return *this;
			}

			void operator += (const EntryPoint& ep)
			{
				m_pEntryPoint = ep;
				m_priority = Priority::NORMAL;
			}
		};

		struct WaitData
		{
			LPVOID fiber = {};
			Counter* pCounter = nullptr;
			int desiredCount = 0;

			WaitData()
				: fiber(0), pCounter(nullptr), desiredCount(0)
			{ }

			WaitData(const LPVOID _fiber, Counter* _counter, const unsigned int _desiredCount)
				: fiber(_fiber), pCounter(_counter), desiredCount(_desiredCount)
			{ }

			// Copy instructions

			WaitData(const WaitData& other)
				: fiber(other.fiber), pCounter(other.pCounter), desiredCount(other.desiredCount)
			{ }

			WaitData& operator=(const WaitData& other)
			{
				fiber = other.fiber;
				pCounter = other.pCounter;
				desiredCount = other.desiredCount;

				return *this;
			}

			// Move instructions
			WaitData(WaitData&& other) noexcept
				: fiber(other.fiber), pCounter(other.pCounter), desiredCount(other.desiredCount)
			{
				// Don't reset other's members because of performance
				// This data is probably overridden by the calling function anyways.
			}

			WaitData& operator=(WaitData&& other) noexcept
			{
				fiber = other.fiber;
				pCounter = other.pCounter;
				desiredCount = other.desiredCount;

				// Don't reset other's members because of performance
				// This data is probably overridden by the calling function anyways.
				return *this;
			}
		};

		void operator += (JobDeclaration& jobDecl)
		{
			KickJob(jobDecl);
		}

		bool KickJobAndFreeDecl(JobDeclaration& decl, Counter* cnt = nullptr);
		bool KickJob(JobDeclaration& decl, Counter* cnt = nullptr);
		bool KickJobAndWait(JobDeclaration& decl, const Counter* waitForCnt);
		bool KickJobs(int count, JobDeclaration decls[], Counter* pJobCounter = nullptr);
		void WaitForCounter(Counter& pJobCounter, const int desiredCnt) const;
		void WaitForCounterAndFree(Counter& pJobCounter, const int desiredCnt) const;
		void BusyWaitForCounter(Counter& pJobCounter, const int desiredCnt) const;
		void BusyWaitForCounterAndFree(Counter& pJobCounter, const int desiredCnt) const;
		void SpawnWorkerThreads(const short numOfThreads = -1);

	public:

		UINT64 GetFiberPoolSize() const
		{
			return fiber_pool.size();
		}

		UINT64 GetUsedFibers() const
		{
			return NUM_FIBERS - GetFiberPoolSize();
		}

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
		static LPVOID RemoveWaitListElement(unsigned int index);

		static SpinLock jobQueueSpinLock;
		static SpinLock fiberSpinLock;
		static SpinLock threadIdSpinLock;
		static SpinLock waitListSpinLock;

	private:

		// Wait list for Fibers and their jobs
		alignas(128) static std::array<WaitData, NUM_FIBERS> wait_list;
		static std::atomic<int> waitListPointer;
		
		// @TODO: Implement custom queue class
		// @TODO allocate in customly in pool alloc
		alignas(128) static std::queue<JobDeclaration> jobQueue_High;
		alignas(128) static std::queue<JobDeclaration> jobQueue_Normal;
		alignas(128) static std::queue<JobDeclaration> jobQueue_Low;

		static LPVOID mainFiber;	// 8 bytes
		alignas(8) static std::array<LPVOID, NUM_FIBERS> fiber_pool;
		alignas(16) static std::unordered_map<DWORD, LPVOID> thread_fibers;

		static std::atomic<unsigned int> fiberPoolPointer;

		// @TODO: Implement custom vector / list class
		std::vector<std::thread*> worker_threads = {};
	};
}

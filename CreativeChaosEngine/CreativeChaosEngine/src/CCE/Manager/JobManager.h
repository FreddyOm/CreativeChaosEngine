#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>

namespace CCE
{
	struct CCE_API JobManager : public BaseManager
	{
	public:
		JobManager() = default;
		~JobManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static JobManager* Instance;
	
	protected:
		struct Job
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

			struct Declaration
			{
				EntryPoint* m_pEntryPoint;
				va_list m_param;
				Priority m_priority;
				Counter* m_pCounter;
			};

			Job(Declaration decl)
			{
				id = g_index++;
				
				m_pEntrancePoint = decl.m_pEntryPoint;
				m_params = decl.m_param;
				m_pCounter = decl.m_pCounter;
				m_priority = decl.m_priority;
			}
			static unsigned int g_index;
			unsigned int id = 0;

			EntryPoint* m_pEntrancePoint = nullptr;
			va_list m_params= nullptr;
			Priority m_priority = Priority::LOW;
			Counter* m_pCounter = nullptr;
		};

		struct Fiber
		{
			struct FiberContext
			{
				// Registers
				// Stack space
			};

			unsigned int id = 0;
			FiberContext cntxt;
		};
		void SpawnWorkerThreads(short numOfThreads = -1);
		bool KickJob(const Job::Declaration& decl);
		bool KickJobs(int count, const Job::Declaration decls[]);
	private:
		void SpawnWorkerThreadsWin(short numOfThreads = -1);
		
	private:
		LPVOID mainFiber = nullptr;
		std::vector<std::thread> worker_threads;
		std::vector<Fiber> fiber_pool;
		std::vector<Job> wait_list;
		
		std::queue<Job> jobQueue_High;
		std::queue<Job> jobQueue_Normal;
		std::queue<Job> jobQueue_Low;
	};
}

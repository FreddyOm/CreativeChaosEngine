#pragma once
#include "BaseManager.h"
#include <thread>
#include <vector>
#include <queue>
#include "../String/String.h"

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
				CCE::String m_Description;	// 8 bytes
				EntryPoint* m_pEntryPoint;	// 4 bytes
				va_list m_param;
				Priority m_priority;
				Counter* m_pCounter;
			};

			Job(Declaration decl)
			{
				id = g_index++;
				
				m_Description = decl.m_Description;
				m_pEntrancePoint = decl.m_pEntryPoint;
				m_params = decl.m_param;
				m_pCounter = decl.m_pCounter;
				m_priority = decl.m_priority;
			}
			
		public:

			static void ResetIdIndex();

		public:

			static unsigned int g_index;
			unsigned int id = 0;

			CCE::String m_Description;
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

			Fiber(unsigned int _id, FiberContext _cntxt)
			{
				id = _id;
				cntxt = _cntxt;
			}

			unsigned int id = 0;
			FiberContext cntxt;
		};
		void SpawnWorkerThreads(const short numOfThreads = -1);
		void PopulateFiberPool(const short numOfFibers = 100);
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
		std::vector<Fiber> fiber_pool;
		std::vector<Job> wait_list;
		
		// TODO: Implement custom queue class
		std::queue<Job> jobQueue_High;
		std::queue<Job> jobQueue_Normal;
		std::queue<Job> jobQueue_Low;
	};
}

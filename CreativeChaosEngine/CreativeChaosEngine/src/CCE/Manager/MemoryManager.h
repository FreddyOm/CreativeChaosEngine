#pragma once
#include "BaseManager.h"
#include "../Memory/PoolAllocator.h"
#include "../Memory/StackAllocator.h"
#include "../CCEditor/CCEditor.h"
#include "../Analysis/Logger.h"

#define JOB_POOL_SIZE 32
#define JOB_POOL_ELMNT_CNT 10

#define DEBUG_POOL_SIZE 128
#define DEBUG_POOL_ELMNT_CNT 256

#define REND_STACK_SIZE 12040

namespace CCE
{
	using namespace CCMemory;

	struct CCE_API MemoryManager :
		public BaseManager
	{
		MemoryManager() = default;

		~MemoryManager() = default;

		void StartUp();
		void ShutDown();

		void UpdateMemoryUsage()
		{
			PUSH_EDITOR_FLOAT("debugMemTotal", debugMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("debugMemUsed", debugMemory.GetUsedMem());

			PUSH_EDITOR_FLOAT("jobMemTotal", jobMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("jobMemUsed", jobMemory.GetUsedMem());

			PUSH_EDITOR_FLOAT("rendMemTotal", rendMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("rendMemUsed", rendMemory.GetUsedMem());			
		}

	public:
		static MemoryManager* Instance;

		PoolAllocator jobMemory = PoolAllocator(JOB_POOL_ELMNT_CNT, JOB_POOL_SIZE);
		PoolAllocator debugMemory = PoolAllocator(DEBUG_POOL_ELMNT_CNT, DEBUG_POOL_SIZE);
		StackAllocator rendMemory = StackAllocator(REND_STACK_SIZE);
	};
}

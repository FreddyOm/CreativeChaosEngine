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
			PUSH_EDITOR_FLOAT("debugMemTotal", (const float) debugMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("debugMemUsed", (const float) debugMemory.GetUsedMem());

			PUSH_EDITOR_FLOAT("jobMemTotal", (const float) jobMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("jobMemUsed", (const float) jobMemory.GetUsedMem());

			PUSH_EDITOR_FLOAT("rendMemTotal", (const float) rendMemory.GetTotalMem());
			PUSH_EDITOR_FLOAT("rendMemUsed", (const float) rendMemory.GetUsedMem());
		}

	public:
		static MemoryManager* Instance;

		PoolAllocator jobMemory = PoolAllocator(JOB_POOL_ELMNT_CNT, JOB_POOL_SIZE);
		PoolAllocator debugMemory = PoolAllocator(DEBUG_POOL_ELMNT_CNT, DEBUG_POOL_SIZE);
		StackAllocator rendMemory = StackAllocator(REND_STACK_SIZE);
	};
}

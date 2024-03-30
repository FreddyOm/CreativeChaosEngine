#pragma once
#include "BaseManager.h"
#include "../memory/pool-allocator.h"
#include "../memory/stack-allocator.h"
#include "../analysis/logger.h"

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

	public:
		static MemoryManager* Instance;

		PoolAllocator jobMemory = PoolAllocator(JOB_POOL_ELMNT_CNT, JOB_POOL_SIZE);
		PoolAllocator debugMemory = PoolAllocator(DEBUG_POOL_ELMNT_CNT, DEBUG_POOL_SIZE);
		StackAllocator rendMemory = StackAllocator(REND_STACK_SIZE);
	};
}

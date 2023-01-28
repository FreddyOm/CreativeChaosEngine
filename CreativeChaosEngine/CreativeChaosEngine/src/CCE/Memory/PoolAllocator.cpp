#include "PoolAllocator.h"
#include <iterator>

namespace CCMemory
{
	unsigned long PoolAllocator::GetPoolSize()
	{
		return poolSize;
	}

	unsigned int PoolAllocator::GetNumPoolElements()
	{
		return numPoolElements;
	}

	unsigned int PoolAllocator::GetNumFreePoolElements()
	{
		return freePoolElements;
	}

	//TODO: Add size into alignment buffer (?) or remove it so that
	// it's not nessecary to give the explicit size of the object to
	// free. This is still error prone since you might give arbitrary
	// values to the function which in consequence breaks the memory
	// counting (free mem / used mem).
	void PoolAllocator::Free(intptr_t addr, size_t size)
	{
		// check
		DASSERT(addr < allocatableMemBottom + totalSpace,
			"Trying to free a memory adress which is not part of the allocated memory!");

		// free
		unsigned int poolIndex = (addr - allocatableMemBottom) / poolSize;
		
		if (pool[poolIndex])
		{
			pool[poolIndex] = false;

			// update
			freePoolElements++;
			numFrees++;
			usedSpace -= size;
			freeSpace += size;

		}
	}

	void PoolAllocator::Clear()
	{
		memset(pool, FALSE, numPoolElements);
		freePoolElements = numPoolElements;
		usedSpace = 0;
		freeSpace = totalSpace;
	}
}

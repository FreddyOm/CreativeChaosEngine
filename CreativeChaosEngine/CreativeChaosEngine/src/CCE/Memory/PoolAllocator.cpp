#include "PoolAllocator.h"
#include <iterator>
#include "../Analysis/Debug.h"
#include <minwindef.h>

namespace CCMemory
{
	/// <summary>
	/// Gets the pool size.
	/// </summary>
	/// <returns>Returns the size of the pools of the allocator.</returns>
	const unsigned long PoolAllocator::GetPoolSize()
	{
		return poolSize;
	}

	/// <summary>
	/// Gets the number of all pool elements.
	/// </summary>
	/// <returns>Returns the number of all available pool elements of the allocator.</returns>
	const unsigned int PoolAllocator::GetNumPoolElements()
	{
		return numPoolElements;
	}

	/// <summary>
	/// Gets the number of the free pool elements.
	/// </summary>
	/// <returns>Returns the number of free pool elements of the allocator.</returns>
	const unsigned int PoolAllocator::GetNumFreePoolElements()
	{
		return freePoolElements;
	}

	//TODO: Add size into alignment buffer (?) or remove it so that
	// it's not nessecary to give the explicit size of the object to
	// free. This is still error prone since you might give arbitrary
	// values to the function which in consequence breaks the memory
	// counting (free mem / used mem).
	void PoolAllocator::Free(intptr_t addr, unsigned int size)
	{
		// check
		DASSERT(addr < allocatableMemBottom + totalSpace,
			"Trying to free a memory adress which is not part of the allocated memory!");

		// free
		intptr_t poolIndex = (addr - allocatableMemBottom) / poolSize;
		
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

	/// <summary>
	/// Clears all of the elements of the pool allocator.
	/// </summary>
	void PoolAllocator::Clear()
	{
		memset(pool, FALSE, numPoolElements);
		freePoolElements = numPoolElements;
		usedSpace = 0;
		freeSpace = totalSpace;
	}
}

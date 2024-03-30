#include "pool-allocator.h"
#include <iterator>
#include "../analysis/debug.h"
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

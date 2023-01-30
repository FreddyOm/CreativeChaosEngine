#include "Allocator.h"
namespace CCMemory
{
	/// <summary>
	/// Gets the total number of allocations.
	/// </summary>
	/// <returns>Returns the total number of allocations of the allocator.</returns>
	const intptr_t Allocator::GetNumAllocs()
	{
		return numAllocs;
	}
	
	/// <summary>
	/// Gets the total number of frees (clear not included).
	/// </summary>
	/// <returns>Returns the number of frees of the allocator in bytes.</returns>
	const intptr_t Allocator::GetNumFrees()
	{
		return numFrees;
	}

	/// <summary>
	/// Gets the currently free memory.
	/// </summary>
	/// <returns>Returns the free memory of the allocator in bytes.</returns>
	const intptr_t Allocator::GetFreeMem()
	{
		return freeSpace;
	}

	/// <summary>
	/// Gets the currently used memory.
	/// </summary>
	/// <returns>Returns the used memory of the allocator in bytes.</returns>
	const intptr_t Allocator::GetUsedMem()
	{
		return usedSpace;
	}

	/// <summary>
	/// Gets the total available memory.
	/// </summary>
	/// <returns>Returns the total available memory of the allocator in bytes.</returns>
	const intptr_t Allocator::GetTotalMem()
	{
		return totalSpace;
	}

	/// <summary>
	/// Gets the ratio of used to total memory.
	/// </summary>
	/// <returns>Returns the used to total memory ratio of the allocator.</returns>
	const float Allocator::GetUsedSpaceRatio()
	{
		return (float)usedSpace / (float)totalSpace;
	}
}

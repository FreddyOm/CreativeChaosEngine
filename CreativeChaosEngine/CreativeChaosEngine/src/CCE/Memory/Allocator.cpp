#include "Allocator.h"
namespace CCMemory
{
	intptr_t Allocator::GetNumAllocs()
	{
		return numAllocs;
	}
	intptr_t Allocator::GetNumFrees()
	{
		return numFrees;
	}
	intptr_t Allocator::GetFreeMem()
	{
		return freeSpace;
	}

	intptr_t Allocator::GetUsedMem()
	{
		return usedSpace;
	}

	intptr_t Allocator::GetTotalMem()
	{
		return totalSpace;
	}

	float Allocator::GetUsedSpaceRatio()
	{
		return (float)usedSpace / (float)totalSpace;
	}
}

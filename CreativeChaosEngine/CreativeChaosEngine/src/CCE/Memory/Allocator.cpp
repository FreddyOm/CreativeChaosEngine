#include "Allocator.h"
namespace CCMemory
{
	unsigned int Allocator::GetNumAllocs()
	{
		return numAllocs;
	}
	unsigned int Allocator::GetNumFrees()
	{
		return numFrees;
	}
	unsigned int Allocator::GetFreeMem()
	{
		return freeSpace;
	}

	unsigned int Allocator::GetUsedMem()
	{
		return usedSpace;
	}

	unsigned int Allocator::GetTotalMem()
	{
		return totalSpace;
	}

	float Allocator::GetUsedSpaceRatio()
	{
		return (float)usedSpace / (float)totalSpace;
	}
}

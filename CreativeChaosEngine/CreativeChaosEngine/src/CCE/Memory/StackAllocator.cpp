#include "StackAllocator.h"
#include "../Core.h"

namespace CCMemory
{
	/// <summary>
	/// Frees the memory to the marker.
	/// </summary>
	void StackAllocator::Free(unsigned long _size)
	{
		if (usedSpace > 0)
		{
			top -= _size;
			usedSpace -= _size;
			UpdateFreeSpace();
			numFrees++;
		}
	}


	/// <summary>
	/// Clears all the memory in the allocator.
	/// </summary>
	void StackAllocator::ClearAll()
	{
		usedSpace = 0;
		freeSpace = totalSpace;
		top = bottom;
	}

	/// <summary>
	/// Returns a pointer to the top of the stack.
	/// </summary>
	/// <returns></returns>
	intptr_t StackAllocator::GetCurretTop()
	{
		return top;
	}

	/// <summary>
	/// Updates the freeSpace member.
	/// </summary>
	void StackAllocator::UpdateFreeSpace()
	{
		freeSpace = totalSpace - usedSpace;
		DASSERT(freeSpace >= 0, "Free space may not be less than zero!");
	}

	/// <summary>
	/// Sets the marker to the current top value
	/// </summary>
	void StackAllocator::RollbackToMarker(StackAllocMarker _marker)
	{
		if (top > _marker)
		{
			usedSpace = (top - bottom);
			UpdateFreeSpace();
			top = _marker;
		}
			
	}

	unsigned int StackAllocator::GetFreeMem()
	{
		return GetTotalMem() - GetUsedMem();
	}
	unsigned int StackAllocator::GetUsedMem()
	{
		return top - bottom;
	}
	unsigned int StackAllocator::GetTotalMem()
	{
		return totalSpace;
	}
	float StackAllocator::GetUsedSpaceRatio()
	{
		return (float)GetUsedMem() / (float)GetTotalMem();
	}
}

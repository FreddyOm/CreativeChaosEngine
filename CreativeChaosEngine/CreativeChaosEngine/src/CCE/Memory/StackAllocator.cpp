#include "StackAllocator.h"
#include "../Core.h"

namespace CCMemory
{
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
	const intptr_t StackAllocator::GetCurretTop()
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
		else 
		{
			DWARNING("You are trying to rollback to a higher mem adress!");
		}
	}

	/// <summary>
	/// Gets the currently free memory.
	/// </summary>
	/// <returns>Returns the free memory of the stack allocator.</returns>
	const intptr_t StackAllocator::GetFreeMem()
	{
		return GetTotalMem() - GetUsedMem();
	}

	/// <summary>
	/// Returns the used memory.
	/// </summary>
	/// <returns>Used memory of the stack allocator.</returns>
	const intptr_t StackAllocator::GetUsedMem()
	{
		return top - bottom;
	}

	/// <summary>
	/// Returns the total available memory.
	/// </summary>
	/// <returns>The total memory available.</returns>
	const intptr_t StackAllocator::GetTotalMem()
	{
		return totalSpace;
	}

	/// <summary>
	/// Shows the ratio of used space to total space.
	/// </summary>
	/// <returns>Ratio used space to total space.</returns>
	const float StackAllocator::GetUsedSpaceRatio()
	{
		return (float)GetUsedMem() / (float)GetTotalMem();
	}
}

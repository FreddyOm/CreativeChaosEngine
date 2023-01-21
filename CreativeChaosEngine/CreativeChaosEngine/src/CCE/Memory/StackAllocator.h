#pragma once
#include "Allocator.h"
#include <memory>

namespace CCMemory
{
	typedef intptr_t StackAllocMarker;
	struct CCE_API StackAllocator : public Allocator
	{
		StackAllocator(unsigned long size)
		{
			totalSpace = size;
			freeSpace = totalSpace;
			bottom = reinterpret_cast<intptr_t>(malloc(size));
			top = bottom;
		}
		
		~StackAllocator()
		{
			totalSpace = 0;
			freeSpace = totalSpace;
			top = 0;
			free(reinterpret_cast<int*>(bottom));
			bottom = 0;
		}

		/// <summary>
		/// Allocates a chunk of memory of size 'size'.
		/// </summary>
		/// <param name="size">The size of the object.</param>
		/// <returns>A pointer to the object.</returns>
		template<typename T>
		T* Alloc(unsigned long _size)
		{
			if (_size > freeSpace)
			{
				DWARNING("The allocator ran out of memory!");
				return nullptr;
			}
			
			T* _ptr = new(reinterpret_cast<T*>(top)) T();
			top += _size;
			
			usedSpace += _size;
			UpdateFreeSpace();

			numAllocs++;

			return _ptr;
		}
		void Free(unsigned long _size);

		void RollbackToMarker(StackAllocMarker _marker);
		void ClearAll();
		intptr_t GetCurretTop();
		unsigned int GetFreeMem() override;
		unsigned int GetUsedMem() override;
		unsigned int GetTotalMem() override;
		float GetUsedSpaceRatio() override;

	private:
		intptr_t bottom = 0;
		intptr_t top = 0;

		void UpdateFreeSpace();
	};
}



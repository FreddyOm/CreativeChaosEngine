#pragma once
#include "Allocator.h"
#include <memory>
#include "../Analysis/Debug.h"

namespace CCMemory
{
	typedef intptr_t StackAllocMarker;
	struct CCE_API StackAllocator : public Allocator
	{
		typedef unsigned char AllocOffset;
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
			free((void*)bottom);
			bottom = 0;
		}

		/// <summary>
		/// Allocates a chunk of memory of size 'size'.
		/// </summary>
		/// <param name="size">The size of the object.</param>
		/// <returns>A pointer to the object.</returns>
		template<typename T>
		T* Alloc()
		{
			const unsigned long _size = sizeof(T);
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

		template<typename T>
		T* AllocAligned()
		{
			const unsigned int _size = sizeof(T);
			const unsigned int alignedSize = _size * 2;

			if (_size*2 > freeSpace)
			{
				DWARNING("The allocator ran out of memory!");
				return nullptr;
			}

			// calc offset
			const char offset = _size - (top % _size);
			DASSERT(_size - (top % _size) <= 255, "The offset is greater than the savable info byte.");
			AllocOffset* pOffset = reinterpret_cast<AllocOffset*>(top) + offset - 1;
			*pOffset = offset;

			// set pointer
			T* _ptr = new(reinterpret_cast<T*>(top + offset)) T();
			top += _size + offset;

			usedSpace += _size;
			UpdateFreeSpace();

			numAllocs++;

			return _ptr;
		}

		void Free(const unsigned long _size);
		void FreeAligned(const unsigned long _size);

		void RollbackToMarker(StackAllocMarker _marker);
		void ClearAll();
		const intptr_t GetCurretTop();
		const intptr_t GetFreeMem() override;
		const intptr_t GetUsedMem() override;
		const intptr_t GetTotalMem() override;
		const float GetUsedSpaceRatio() override;

	private:
		intptr_t bottom = 0;
		intptr_t top = 0;

		void UpdateFreeSpace();
	};
}

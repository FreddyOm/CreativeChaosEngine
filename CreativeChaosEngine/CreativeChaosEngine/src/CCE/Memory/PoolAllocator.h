#pragma once
#include "Allocator.h"
#include <memory>
#include "../Analysis/Debug.h"

namespace CCMemory
{
	typedef unsigned char AllocOffset;
	struct CCE_API PoolAllocator : public Allocator
	{
		PoolAllocator(const unsigned int elements, const unsigned long _poolSize)
		{
			DASSERT(elements >= 0, "The element length must be at least 1!");
			DASSERT(_poolSize >= 0, "The pool must be at least 1!");

			totalSpace = (intptr_t)(_poolSize * elements);
			this->poolSize = _poolSize;
			freeSpace = totalSpace;
			numPoolElements = elements;

			freePoolElements = elements;
			
			// malloc [ poolIndices | pe0 |  pe1 | pe2 | ... | pen ]
			bottom = (intptr_t) malloc((sizeof(bool) * elements) + totalSpace);
			pool = new (reinterpret_cast<bool*>(bottom)) bool[elements];
			memset(pool, FALSE, elements);
			allocatableMemBottom = bottom + ((sizeof(bool) * numPoolElements));
		}

		~PoolAllocator()
		{
			poolSize = 0;
			totalSpace = 0;
			freeSpace = 0;
			usedSpace = 0;
			numAllocs = 0;
			numFrees = 0;
			free((void*)bottom);
			numPoolElements = 0;
			bottom = 0;
		}

		template<typename T>
		T* Alloc()
		{
			// check
			DASSERT(sizeof(T) <= poolSize,
				"The size of an allocated chunk of memory must be smaller than the specified pool size.");
			if (freePoolElements <= 0)
			{
				DWARNING("Pool allocator ran out of pool elements!");
				return nullptr;
			}

			// allocate
			unsigned int elementIndex = 0;
			T* ptr = 0;
			for (intptr_t elementStatus = (intptr_t)pool; elementStatus < (intptr_t)pool + numPoolElements; elementStatus += sizeof(bool))
			{
				if (*(bool*)elementStatus == false)
				{
					// element not used yet
					intptr_t adress = (allocatableMemBottom +
						poolSize * elementIndex);
					ptr = new (reinterpret_cast<T*> (adress)) T();
					*(bool*)elementStatus = true;
					break;
				}
				elementIndex++;
			}

			if (ptr == 0)
			{
				// probably no pool element free
				DWARNING("Pool Allocator ran out of pool elements!");
				return nullptr;
			}

			// update
			numAllocs++;
			freePoolElements--;
			usedSpace += sizeof(T);
			freeSpace = totalSpace - usedSpace;

			return ptr;
		}

		template<typename T>
		T* AllocAligned(unsigned short _alignas = -1)
		{
			// check
			if (freePoolElements <= 0)
			{
				DWARNING("Pool allocator ran out of pool elements!");
				return nullptr;
			}

			const unsigned long typesize = sizeof(T);

			if (_alignas == -1)
			{
				_alignas = typesize;
			}

			unsigned long aligned_size = typesize * 2;

			DASSERT(aligned_size <= poolSize,
				"The size of an allocated chunk of memory must be smaller than the specified pool size.");

			// allocate
			unsigned int elementIndex = 0;
			T* ptr = 0;
			for (intptr_t elementStatus = (intptr_t)pool; elementStatus < (intptr_t)pool + numPoolElements; elementStatus += sizeof(bool))
			{
				if (*(bool*)elementStatus == false)
				{
					// element not used yet
					intptr_t adress = (allocatableMemBottom +
						poolSize * elementIndex);

					// calc offset

					intptr_t offset = (typesize - (adress % typesize));
					DASSERT(offset <= 255, "The allocators alignment offset is too big!");

					// align ptr
					adress += offset;

					AllocOffset* pOffset = new (reinterpret_cast<AllocOffset*>(adress -1)) AllocOffset();
					*pOffset = offset;
					ptr = new (reinterpret_cast<T*> (adress)) T();
					*(bool*)elementStatus = true;
					break;
				}
				elementIndex++;
			}

			if (ptr == 0)
			{
				// probably no pool element free
				DWARNING("Pool Allocator ran out of pool elements!");
				return nullptr;
			}

			// update
			numAllocs++;
			freePoolElements--;
			usedSpace += sizeof(T);
			freeSpace = totalSpace - usedSpace;

			return ptr;
		}

		void Free(const intptr_t adr, const unsigned int size);
		void FreeAligned(const intptr_t adr, const unsigned int size);
		void Clear();
		const unsigned long GetPoolSize();
		const unsigned int GetNumPoolElements();
		const unsigned int GetNumFreePoolElements();

	private:
		unsigned long poolSize = 0;
		unsigned int freePoolElements = 0;
		unsigned int numPoolElements = 0;
		bool* pool = nullptr;
		intptr_t bottom = 0;
		intptr_t allocatableMemBottom = 0;
	};
}

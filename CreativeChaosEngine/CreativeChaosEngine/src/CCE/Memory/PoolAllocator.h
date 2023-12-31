#pragma once
#include <memory>
#include "Allocator.h"
#include "../Analysis/Debug.h"

namespace CCMemory
{
	struct CCE_API PoolAllocator : public Allocator
	{
		typedef unsigned char AllocOffset;
		PoolAllocator(const unsigned int elements, const unsigned long _poolSize)
			: poolSize(_poolSize), freePoolElements(elements), numPoolElements(elements)
		{
			DASSERT(elements >= 0, "The element length must be at least 1!");
			DASSERT(_poolSize >= 0, "The pool must be at least 1!");

			totalSpace = (intptr_t)(_poolSize * elements);
			freeSpace = totalSpace;
			
			// malloc [ poolIndices | pe0 |  pe1 | pe2 | ... | peN ]
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
			for (intptr_t elementStatus = reinterpret_cast<intptr_t>(pool); elementStatus < reinterpret_cast<intptr_t>(pool + numPoolElements); elementStatus += sizeof(bool))
			{
				if (*(bool*)elementStatus == false)
				{
					// element not used yet
					intptr_t adress = (allocatableMemBottom +
						(intptr_t)poolSize * elementIndex);
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
		T* AllocAligned(unsigned short defaultAlignSize = 8, ...)
		{
			// check
			if (freePoolElements <= 0)
			{
				DWARNING("Pool allocator ran out of pool elements!");
				return nullptr;
			}

			const unsigned long typesize = sizeof(T);

			if (typesize > defaultAlignSize)
			{
				defaultAlignSize = typesize;
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
						(intptr_t)poolSize * elementIndex);

					// calc offset

					intptr_t offset = (typesize - (adress % typesize));
					DASSERT(offset <= 255, "The allocators alignment offset is too big!");

					// align ptr
					adress += offset;

					AllocOffset* pOffset = (AllocOffset*)(adress -1);
					*pOffset = static_cast<unsigned char>(offset);
					va_list args;
					va_start(args, defaultAlignSize);
					ptr = reinterpret_cast<T*> (adress);
					*(bool*)elementStatus = true;
					va_end(args);
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
		void FreeAligned(const T* adr)
		{
			// check
			DASSERT((intptr_t)adr < allocatableMemBottom + totalSpace,
				"Trying to free a memory adress which is not part of the allocated memory!");

			// calc actual start adress
			AllocOffset* pOffset = reinterpret_cast<AllocOffset*> ((intptr_t)adr - 1);
			intptr_t poolStartAdress = (intptr_t)adr - *pOffset;

			// free
			intptr_t poolIndex = (poolStartAdress - allocatableMemBottom) / poolSize;

			if (pool[poolIndex])
			{
				pool[poolIndex] = false;

				// update
				freePoolElements++;
				numFrees++;
				usedSpace -= sizeof(T);
				freeSpace += sizeof(T);
			}
		}

		template<typename T>
		void Free(const T* adr)
		{
			// check
			DASSERT(reinterpret_cast<intptr_t>(adr) < allocatableMemBottom + totalSpace,
				"Trying to free a memory adress which is not part of the allocated memory!");

			// free
			intptr_t poolIndex = (reinterpret_cast<intptr_t>(adr) - allocatableMemBottom) / poolSize;

			if (pool[poolIndex])
			{
				pool[poolIndex] = false;

				// update
				freePoolElements++;
				numFrees++;
				usedSpace -= sizeof(T);
				freeSpace += sizeof(T);
			}
		}
		
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

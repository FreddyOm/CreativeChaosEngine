#pragma once
#include "../Core.h"

namespace CCMemory
{
	//TODO: Mal schauen, ob die Allocators mit exportiert werden müssen
	struct CCE_API Allocator
	{
		Allocator() = default;
		~Allocator() = default;

	protected:
		intptr_t totalSpace = 0;
		intptr_t freeSpace = 0;
		intptr_t usedSpace = 0;
		intptr_t numAllocs = 0;
		intptr_t numFrees = 0;

	public:
		intptr_t GetNumAllocs();
		intptr_t GetNumFrees();
		virtual intptr_t GetFreeMem();
		virtual intptr_t GetUsedMem();
		virtual intptr_t GetTotalMem();
		virtual float GetUsedSpaceRatio();
	};
}



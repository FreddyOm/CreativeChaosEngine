#pragma once
#include "../Core.h"

namespace CCMemory
{
	// @TODO: Mal schauen, ob die Allocators mit exportiert werden müssen
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
		const intptr_t GetNumAllocs();
		const intptr_t GetNumFrees();
		const virtual intptr_t GetFreeMem();
		const virtual intptr_t GetUsedMem();
		const virtual intptr_t GetTotalMem();
		const virtual float GetUsedSpaceRatio();
	};
}



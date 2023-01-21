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
		unsigned int totalSpace = 0;
		unsigned int freeSpace = 0;
		unsigned int usedSpace = 0;
		unsigned int numAllocs = 0;
		unsigned int numFrees = 0;

	public:
		unsigned int GetNumAllocs();
		unsigned int GetNumFrees();
		virtual unsigned int GetFreeMem();
		virtual unsigned int GetUsedMem();
		virtual unsigned int GetTotalMem();
		virtual float GetUsedSpaceRatio();
	};
}



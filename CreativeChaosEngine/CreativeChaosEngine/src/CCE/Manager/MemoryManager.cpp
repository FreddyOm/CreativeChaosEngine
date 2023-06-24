#include "MemoryManager.h"

namespace CCE
{
	MemoryManager* MemoryManager::Instance = nullptr;
	
	void MemoryManager::StartUp()
	{
		if (initialized) { return; }
		initialized = true;
		Instance = this;

	}

	void MemoryManager::ShutDown()
	{
		if (!initialized) { return; }
		initialized = false;
		Instance = nullptr;
		jobMemory.Clear();
		debugMemory.Clear();
		rendMemory.ClearAll();
	}
}
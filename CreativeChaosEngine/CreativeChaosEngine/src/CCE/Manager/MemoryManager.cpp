#include "MemoryManager.h"
#include "../Analysis/Logger.h"

namespace CCE
{
	MemoryManager* MemoryManager::Instance = nullptr;
	
	void MemoryManager::StartUp()
	{
		if (initialized) { return; }
		initialized = true;
		Instance = this;
		LOGC("MemoryManager initialized!", COLOR_BLUE);
	}

	void MemoryManager::ShutDown()
	{
		if (!initialized) { return; }
		initialized = false;
		jobMemory.Clear();
		debugMemory.Clear();
		rendMemory.ClearAll();
		LOGC("Shutting down MemoryManager...", COLOR_BLUE);
		Instance = nullptr;
	}
}
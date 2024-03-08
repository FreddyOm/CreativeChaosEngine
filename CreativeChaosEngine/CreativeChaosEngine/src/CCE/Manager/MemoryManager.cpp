#include "MemoryManager.h"
#include "../Analysis/Logger.h"

namespace CCE
{
	MemoryManager* MemoryManager::Instance = nullptr;
	
	void MemoryManager::StartUp()
	{
		if (BaseManager::IsInitialized()) { return; }
		BaseManager::Init();
		Instance = this;
		LOGC("MemoryManager initialized!", COLOR_BLUE);
	}

	void MemoryManager::ShutDown()
	{
		if (!BaseManager::IsInitialized()) { return; }
		BaseManager::Deinit();
		jobMemory.Clear();
		debugMemory.Clear();
		rendMemory.ClearAll();
		LOGC("Shutting down MemoryManager...", COLOR_BLUE);
		Instance = nullptr;
	}
}
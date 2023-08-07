#include "ProfilingManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void ProfilingManager::StartUp()
	{
		DASSERT(Instance == nullptr, "ProfilingManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		memLeakTable = new std::unordered_map<unsigned long long, int>();

		initialized = true;
		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("ProfilingManager initialized!", COLOR_BLUE);
	}

	void ProfilingManager::ShutDown()
	{
		LOGC("Shutting down ProfilingManager...", COLOR_BLUE);
		initialized = false;
		delete memLeakTable;
		Instance = nullptr;
	}

	/// <summary>
	/// Register an instance increases the reference count for the given instance in the porfiling manager.
	/// </summary>
	/// <param name="name">Use the nameof(*this) in order to always unregister the correct instance.</param>
	void ProfilingManager::RegisterInstance(String name) noexcept
	{
		if (name.sId == NULL) { name = "EXCEPT_DEFAULT_NAME"; }
		if (memLeakTable->find(name.sId) == memLeakTable->end())
		{
			// No such instance in here yet
			memLeakTable->emplace(name.sId, 1);
		}
		else 
		{
			// Increase the instance count for this name
			memLeakTable->insert_or_assign(name.sId, memLeakTable->at(name.sId) + 1);
		}
	}

	/// <summary>
	/// Unregister an instance decreases the reference count for the given reference in the porfiling manager.
	/// </summary>
	/// <param name="name">Use the nameof(*this) in order to always unregister the correct instance.</param>
	void ProfilingManager::UnregisterInstance(String name) noexcept
	{
		DASSERT(memLeakTable->find(name.sId) != memLeakTable->end(),
			"The instance %s you try to unregister was never reigstered!", name.Value());
		
		// Increase the instance count for this name
		memLeakTable->insert_or_assign(name.sId, memLeakTable->at(name.sId) - 1);
	}

	/// <summary>
	/// Only call this method at the very end of the application! 
	/// Otherwise there may be still be instances which have not been yet deconstructed.
	/// </summary>
	void ProfilingManager::PrintLeakInfo() const noexcept
	{
		
		LOG_PROFILING("## ------------- MEMORY LEAK INFO ------------- ##", COLOR_WHITE);

		std::unordered_map<unsigned long long, int>::iterator it;
		for (it = memLeakTable->begin(); it != memLeakTable->end(); it++)
		{
			if (it->second != 0)
			{
				const char* className = String::ValueBySID(it->first);
				LOG_PROFILING("Memory leak detected on object \"%s\". RefCount: %d", 
					COLOR_RED, className, it->second);
			}
		}

		LOG_PROFILING("## -------------------------------------------- ##", COLOR_WHITE);
	}

	ProfilingManager* ProfilingManager::Instance = nullptr;
}

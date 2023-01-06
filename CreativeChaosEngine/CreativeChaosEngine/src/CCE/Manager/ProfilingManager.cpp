#include "ProfilingManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Core.h"
namespace CCE
{
	void ProfilingManager::StartUp()
	{
		if (initialized) 
		{
			DWARNING("ProfilingManager has already been initialized. Returning.");
			return;
		}

		LOGC("Starting Profiling Manager...", COLOR_BLUE);

		initialized = true;
	}
	void ProfilingManager::ShutDown()
	{
		LOGC("Shutting down Profiling Manager...", COLOR_BLUE);
	}
}

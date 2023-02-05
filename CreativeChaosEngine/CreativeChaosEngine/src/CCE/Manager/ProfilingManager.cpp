#include "ProfilingManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void ProfilingManager::StartUp()
	{
		auto startTime = Time::CurrentTick();
		if (initialized) 
		{
			DWARNING("ProfilingManager has already been initialized!");
			return;
		}
		LOGC("Starting ProfilingManager...", COLOR_BLUE);

		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("ProfilingManager initialization done in...", COLOR_BLUE);
		LOGC(initDuration, COLOR_BLUE);
	}
	void ProfilingManager::ShutDown()
	{
		LOGC("Shutting down ProfilingManager...", COLOR_BLUE);
	}
}

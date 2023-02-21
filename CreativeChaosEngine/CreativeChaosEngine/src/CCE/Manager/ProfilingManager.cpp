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
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("ProfilingManager initialized!", COLOR_BLUE);
	}

	void ProfilingManager::ShutDown()
	{
		LOGC("Shutting down ProfilingManager...", COLOR_BLUE);
		initialized = false;
		Instance = nullptr;
	}

	ProfilingManager* ProfilingManager::Instance = nullptr;
}

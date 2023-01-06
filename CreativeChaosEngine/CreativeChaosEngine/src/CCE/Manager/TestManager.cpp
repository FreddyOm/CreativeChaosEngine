#include "TestManager.h"
#if defined(DEBUG) || defined(DEBUG_PROFILE)
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"
#endif // DEBUG

namespace CCE
{
	void TestManager::StartUp()
	{
		auto startTime = Time::CurrentTick();
		if (initialized)
		{
			DWARNING("TestManager has already been initialized!");
			return;
		}
		LOGC("Starting TestManager...", COLOR_BLUE);

		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("TestManager initialization done in...", COLOR_BLUE);
		LOGC(initDuration, COLOR_BLUE);
	}

	void TestManager::ShutDown()
	{
		LOGC("Shutting down TestManager...", COLOR_BLUE);
	}
}
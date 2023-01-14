#include "PhysicsManager.h"
#include "../Analysis/Time.h"
#if defined(DEBUG) || defined(DEBUG_PROFILE)
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#endif // DEBUG

namespace CCE
{
	void PhysicsManager::StartUp()
	{
		auto startTime = Time::CurrentTick();
		if (initialized)
		{
			DWARNING("PhysicsManager has already been initialized!");
			return;
		}
		LOGC("Starting PhysicsManager...", COLOR_BLUE);

		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("PhysicsManager initialization done in..." ,COLOR_BLUE);
		LOGC(initDuration ,COLOR_BLUE);
	}
	void PhysicsManager::ShutDown()
	{
		LOGC("Shutting down PhysicsManager...", COLOR_BLUE);
	}
}
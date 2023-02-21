#include "PhysicsManager.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void PhysicsManager::StartUp()
	{
		DASSERT(Instance == nullptr, "PhysicsManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("PhysicsManager initialized!", COLOR_BLUE);
	}

	void PhysicsManager::ShutDown()
	{
		LOGC("Shutting down PhysicsManager...", COLOR_BLUE);
		initialized = false;
		Instance = nullptr;
	}

	PhysicsManager* PhysicsManager::Instance = nullptr;
}
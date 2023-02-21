#include "RenderManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void RenderManager::StartUp()
	{
		DASSERT(Instance == nullptr, "RenderManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("RenderManager initialized!", COLOR_BLUE);
	}
	void RenderManager::ShutDown()
	{
		LOGC("Shutting down RenderManager...", COLOR_BLUE);
		initialized = false;
		Instance = nullptr;
	}
	
	RenderManager* RenderManager::Instance = nullptr;
}

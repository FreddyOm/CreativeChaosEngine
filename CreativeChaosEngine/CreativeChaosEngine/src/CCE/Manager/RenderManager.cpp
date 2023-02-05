#include "RenderManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

namespace CCE
{
	void RenderManager::StartUp()
	{
		auto startTime = Time::CurrentTick();
		if (initialized)
		{
			DWARNING("RenderManager has already been initialized!");
			return;
		}
		LOGC("Starting RenderManager...", COLOR_BLUE);

		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("RenderManager initialization done in...", COLOR_BLUE);
		LOGC(initDuration, COLOR_BLUE);
	}
	void RenderManager::ShutDown()
	{
		LOGC("Shutting down RenderManager...", COLOR_BLUE);
	}
}


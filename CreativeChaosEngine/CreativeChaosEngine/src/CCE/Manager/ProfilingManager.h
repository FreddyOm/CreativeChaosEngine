#pragma once
#include "BaseManager.h"
#include "../Analysis/Logger.h"

namespace CCE
{
	struct CCE_API ProfilingManager : public BaseManager
	{
	public:
		ProfilingManager() = default;
		~ProfilingManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static ProfilingManager* Instance;

		void TestFunc()
		{
			Logger::Log("Test", COLOR_WHITE, CCE::LogLevel::NONE);
		}
	};
}

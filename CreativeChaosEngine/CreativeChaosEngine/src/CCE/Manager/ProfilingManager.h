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

		void One()
		{
			Logger::Log("Test: 1", COLOR_WHITE, CCE::LogLevel::JOBS);
		}

		void Two()
		{
			Logger::Log("Test: 2", COLOR_WHITE, CCE::LogLevel::JOBS);
		}

		void Three()
		{
			Logger::Log("Test: 3", COLOR_WHITE, CCE::LogLevel::JOBS);
		}

		void Four()
		{
			int l = 0;
			for (int i = 0; i < 100; i++)
			{
				l += i % 3;
			}
			Logger::Log("Test: 4 -> %i", COLOR_WHITE, CCE::LogLevel::JOBS, l);
		}
	};
}

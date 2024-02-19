#pragma once
#include "BaseManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Time.h"
#include <vector>

namespace CCE
{
#if defined DEBUG || defined DEBUG_PROFILE
#define REGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->RegisterInstance(typeid(*this).name())
#define UNREGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->UnregisterInstance(typeid(*this).name())
#define PRINT_LEAK_INFO CCE::ProfilingManager::Instance->PrintLeakInfo()
#define PROFILE_FUNCTION CCE::ProfilingManager::ScopedDataCollector col(__FUNCTION__)

#else

#define REGISTER_LEAK_DETECT
#define UNREGISTER_LEAK_DETECT
#define PRINT_LEAK_INFO
#define PROFILE_FUNCTION

#endif
	struct CCE_API ProfilingManager : public BaseManager
	{
	public:
		ProfilingManager()
		{
			StartUp();
		}
		~ProfilingManager()
		{
			ShutDown();
		}

		void StartUp() override;
		void ShutDown() override;

		static ProfilingManager* Instance;

	public:
		std::unordered_map<unsigned long long, int>* memLeakTable;
		void RegisterInstance(String name) noexcept;
		void UnregisterInstance(String name) noexcept;

		void PrintLeakInfo() const noexcept;

	private:

		struct ProfileData
		{
			String functionName;
			long executionTime;
		};

	public:

		struct ScopedDataCollector
		{
			ScopedDataCollector(String funcName)
				: data({funcName, -1})
			{
				start = Time::Now();
			}

			~ScopedDataCollector()
			{
				end = Time::Now();
				data.executionTime = Time::GetDurationInMicroSec(start, end);
				CCE::ProfilingManager::Instance->m_profileData.push_back(data);
			}

		private:

			std::chrono::steady_clock::time_point start{};
			std::chrono::steady_clock::time_point end{};
			ProfileData data;
		};

		std::vector<ProfileData> m_profileData{};
	};
}

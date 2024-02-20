#pragma once
#include "BaseManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Time.h"
#include <unordered_map>
#include <string>

namespace CCE
{
#if defined DEBUG || defined DEBUG_PROFILE
#define REGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->RegisterInstance(typeid(*this).name())
#define UNREGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->UnregisterInstance(typeid(*this).name())
#define PRINT_LEAK_INFO CCE::ProfilingManager::Instance->PrintLeakInfo()
#if 1
#define PROFILE_FUNCTION CCE::ProfilingManager::ScopedDataCollector prfCollector(__FUNCTION__)
#else
#define PROFILE_FUNCTION
#endif
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

		void StartFrameDebuggerFrame()
		{
			++sampleCount;
			for (auto& kvp : m_profileData)
				kvp.second.Reset();
		}

		void ResetFrameDebugger()
		{
			m_profileData.clear();
			sampleCount = 0;
		}

		struct ProfilingData
		{
			String funcName;
			long minExecTime = 0;
			long maxExecTime = 0;
			long callsPerFrame = 0;
			long totalCalls = 0;

			void Reset()
			{
				callsPerFrame = 0;
			}
		};

		class ScopedDataCollector
		{
		public:
			ScopedDataCollector(std::string funcName)
				: funcName(funcName)
			{
				start = Time::Now();
			}

			~ScopedDataCollector()
			{
				end = Time::Now();
				long execTime = Time::GetDurationInMicroSec(start, end);

				if (CCE::ProfilingManager::Instance->m_profileData.find(funcName) == CCE::ProfilingManager::Instance->m_profileData.end())
				{
					CCE::ProfilingManager::Instance->m_profileData.emplace(
						funcName, 
						ProfilingData{ funcName.c_str(), execTime, execTime, 1, 1}
					);
				}
				else 
				{
					CCE::ProfilingManager::Instance->m_profileData[funcName].minExecTime =
						CCE::ProfilingManager::Instance->m_profileData[funcName].minExecTime > execTime ?
						execTime : CCE::ProfilingManager::Instance->m_profileData[funcName].minExecTime;

					CCE::ProfilingManager::Instance->m_profileData[funcName].maxExecTime =
						CCE::ProfilingManager::Instance->m_profileData[funcName].maxExecTime < execTime ?
						execTime : CCE::ProfilingManager::Instance->m_profileData[funcName].maxExecTime;

					++CCE::ProfilingManager::Instance->m_profileData[funcName].callsPerFrame;
					++CCE::ProfilingManager::Instance->m_profileData[funcName].totalCalls;
				}
			}

		private:

			std::chrono::steady_clock::time_point start{};
			std::chrono::steady_clock::time_point end{};
			std::string funcName;
		};

		std::unordered_map<std::string, ProfilingData> m_profileData{};
		int sampleCount = 0;
	};
}

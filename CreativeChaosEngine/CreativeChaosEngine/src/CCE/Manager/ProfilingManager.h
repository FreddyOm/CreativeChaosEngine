#pragma once
#include "BaseManager.h"
#include "../Analysis/Logger.h"

namespace CCE
{
#if defined DEBUG || defined DEBUG_PROFILE
#define REGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->RegisterInstance(typeid(*this).name())
#define UNREGISTER_LEAK_DETECT CCE::ProfilingManager::Instance->UnregisterInstance(typeid(*this).name())
#define PRINT_LEAK_INFO CCE::ProfilingManager::Instance->PrintLeakInfo()

#else

#define REGISTER_LEAK_DETECT
#define UNREGISTER_LEAK_DETECT
#define PRINT_LEAK_INFO

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
	};
}

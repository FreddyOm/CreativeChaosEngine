#pragma once
#include "BaseManager.h"

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
	};
}

#pragma once
#include "BaseManager.h"

namespace CCE
{
	class CCE_API ProfilingManager : public BaseManager
	{
	public:
		ProfilingManager() = default;
		~ProfilingManager() = default;

		void StartUp() override;
		void ShutDown() override;
	};
}

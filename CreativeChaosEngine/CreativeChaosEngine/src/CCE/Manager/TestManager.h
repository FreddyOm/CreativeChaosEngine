#pragma once
#include "BaseManager.h"

namespace CCE
{
	struct CCE_API TestManager : public BaseManager
	{
	public:
		TestManager() = default;
		~TestManager() = default;

		void StartUp() override;
		void ShutDown() override;
	};
}



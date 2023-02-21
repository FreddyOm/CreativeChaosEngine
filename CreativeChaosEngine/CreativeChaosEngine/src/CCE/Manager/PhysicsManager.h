#pragma once
#include "BaseManager.h"

namespace CCE
{
	struct CCE_API PhysicsManager : public BaseManager
	{
	public:
		PhysicsManager() = default;
		~PhysicsManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static PhysicsManager* Instance;
	};
}



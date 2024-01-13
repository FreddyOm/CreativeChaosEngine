#pragma once
#include "ECSSystem.h"

namespace CCE::ECS::Systems
{
	struct PhysicsSystem : public ECSSystem
	{
	public:
		PhysicsSystem() = default;
		~PhysicsSystem() = default;

		void StartUp();
		void ShutDown();

		void UpdateSystem();
	};
}

#pragma once
#include "ECSSystem.h"

namespace CCE::ECS::Systems
{
	struct RenderingSystem : public ECSSystem
	{
		RenderingSystem() = default;
		~RenderingSystem() = default;

		void StartUp();
		void ShutDown();

		void RegisterEntity(long long entity);

		void UpdateSystem();

	private:

		void UpdateECSBasic();
	};
}

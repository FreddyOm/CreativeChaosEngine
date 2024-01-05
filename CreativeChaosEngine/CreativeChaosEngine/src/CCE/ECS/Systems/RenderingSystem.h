#pragma once
#include "ECSSystem.h"

namespace CCE::ECS::Systems
{
	struct RenderingSystem : public ECSSystem
	{
		RenderingSystem();
		~RenderingSystem();

		void UpdateSystem();
	};
}

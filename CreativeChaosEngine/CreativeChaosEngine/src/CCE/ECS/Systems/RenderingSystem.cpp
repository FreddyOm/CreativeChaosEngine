#include "RenderingSystem.h"
#include "../EntityComponentSystem.h"

namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	RenderingSystem::RenderingSystem()
	{
		ECS::Instance->RegisterSystem<RenderingSystem>();
	}

	RenderingSystem::~RenderingSystem()
	{
		// Nothing yet
	}

	void RenderingSystem::UpdateSystem()
	{
		// @TODO: Implement here!
	}
}

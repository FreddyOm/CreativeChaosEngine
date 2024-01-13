#include "RenderingSystem.h"
#include "../Components/ComponentHeaders.h"
#include "../EntityComponentSystem.h"
#include "../../Analysis/Logger.h"
#include "../Entity.h"

namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	void RenderingSystem::StartUp()
	{
		ECS::Instance->RegisterSystem<RenderingSystem>();
		LOGC("RenderingSystem initialized!", COLOR_BLUE);
	}

	void RenderingSystem::ShutDown()
	{
		LOGC("RenderingSystem uninitialized!", COLOR_BLUE);
	}

	void RenderingSystem::UpdateSystem()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		

		for (long long id : mEntities)
		{
			Entity e = Entity(id);
			// @TODO: Implement here!
		}
	}
}

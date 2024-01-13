#include "PhysicsSystem.h"
#include "../Entity.h"
#include "../EntityComponentSystem.h"
#include "../Components/ComponentHeaders.h"
#include "../../Analysis/Logger.h"

namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	void PhysicsSystem::StartUp()
	{
		ECS::Instance->RegisterSystem<PhysicsSystem>();
		LOGC("PhysicsSystem initialized!", COLOR_BLUE);
	}

	void PhysicsSystem::ShutDown()
	{
		LOGC("PhysicsSystem uninitialized!", COLOR_BLUE);
	}

	void PhysicsSystem::UpdateSystem()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		// @TODO: Implement :)

		// Do physics
		for (long long id : mEntities)
		{
			Entity e = Entity(id);
			e.GetComponent<Rigidbody>()->AddForce({0, - GravitationalAccelarion(), 0 });
		}
	}
}

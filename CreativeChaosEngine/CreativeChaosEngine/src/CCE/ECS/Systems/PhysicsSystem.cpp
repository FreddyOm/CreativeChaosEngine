#include "PhysicsSystem.h"
#include "../Entity.h"
#include "../EntityComponentSystem.h"
#include "../Components/ComponentHeaders.h"


namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	PhysicsSystem::PhysicsSystem()
	{
		ECS::Instance->RegisterSystem<PhysicsSystem>();
	}

	PhysicsSystem::~PhysicsSystem()
	{
		// Nothing yet
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
			// e.GetComponent<Rigidbody>()->AddForce({0, - GravitationalAccelarion(), 0 });
		}
	}
}


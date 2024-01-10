#include "PhysicsSystem.h"
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
		// @TODO: Implement :)

		// Do physics
		for (auto& entity : mEntities)
		{
			//entity.GetComponent<Components::Rigidbody>().AddForce({ 0, -Components::GravitationalAccelarion(), 0 });
		}
	}
}


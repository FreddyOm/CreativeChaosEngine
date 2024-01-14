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

	void PhysicsSystem::RegisterEntity(long long entity)
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		mEntities.insert(entity);
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

			Rigidbody* rb = e.GetComponent<Rigidbody>();
			Transform* tf = e.GetComponent<Transform>();

			rb->AddForce({0, - GravitationalAccelarion(), 0 });

			auto pos = DirectX::XMLoadFloat3(&tf->Position());

			DirectX::XMFLOAT3 newPos{};
			DirectX::XMStoreFloat3(&newPos, DirectX::XMVectorAdd(pos, rb->Velocity()));
			
			tf->SetTranslation(std::move(newPos));
		}
	}
}

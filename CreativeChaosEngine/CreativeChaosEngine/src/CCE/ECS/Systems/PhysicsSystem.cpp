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

		for (long long id : mEntities)
		{
			Entity e = Entity(id);

			auto* tf = e.GetComponent<Transform>();
			auto* rb = e.GetComponent<Rigidbody>();
			rb->AddForce({ 0, -(GravitationalAccelarion() * 0.01f), 0 });

			DirectX::XMFLOAT3 newPos{};
			auto _newPos = DirectX::XMVectorAdd({ tf->Position().x, tf->Position().y, tf->Position().z },
				DirectX::XMVectorScale(rb->Velocity(), Time::deltaTime));

			DirectX::XMStoreFloat3(&newPos, _newPos);

			tf->SetTranslation(std::move(newPos));
		}		
	}
}

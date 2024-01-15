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
		UpdateECSBasic();
	}

	void PhysicsSystem::UpdateECSBasic()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;

		for (long long id : mEntities)
		{
			Entity e = Entity(id);

			auto* tf = e.GetComponent<Transform>();
			auto* rb = e.GetComponent<Rigidbody>();

			// Update rigidbody internal acceleration
			
			rb->UpdateRigidbody();

			if (rb->useGravity)
			{
				// Take rigidbody velocity to update transform
				DirectX::XMFLOAT3 newPos{};
				auto _newPos = DirectX::XMVectorAdd({ tf->Position().x, tf->Position().y, tf->Position().z },
					DirectX::XMVectorScale(rb->Velocity(), Time::deltaTime));

				DirectX::XMStoreFloat3(&newPos, _newPos);
				tf->SetTranslation(std::move(newPos));
			}			

			// @TODO: Now do collision testing...

			BroadPhaseCollisionDetection();
			MidPhaseCollisionDetection();
			NarrowPhaseCollisionDetection();
		}
	}

	/// <summary>
	/// The broad phase of the collision system.
	/// </summary>
	void PhysicsSystem::BroadPhaseCollisionDetection() const
	{

	}

	/// <summary>
	/// The mid phase of the collision system.
	/// </summary>
	void PhysicsSystem::MidPhaseCollisionDetection() const
	{

	}

	/// <summary>
	/// The narrow phase of the collision system.
	/// </summary>
	void PhysicsSystem::NarrowPhaseCollisionDetection() const
	{

	}

	std::vector<long long> PhysicsSystem::collisionSystem{};
}

#include "PhysicsSystem.h"
#include "../Entity.h"
#include "../../Analysis/Logger.h"
#include "../EntityComponentSystem.h"
#include "../Components/ComponentHeaders.h"
#include "../../Utilities/Containers/Octree.h"
#include "../../Utilities/Math/CCMath.h"

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
		auto start = Time::Now();
		UpdateECSBasic();
		auto end = Time::Now();
		PhysicsCalcDuration = Time::GetDurationInMilliSec(start, end);
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
			
			// Take rigidbody velocity to update position
			DirectX::XMFLOAT3 newPos{};
			auto _newPos = DirectX::XMVectorAdd({ tf->Position().x, tf->Position().y, tf->Position().z },
				DirectX::XMVectorScale(rb->Velocity(), Time::deltaTime));

			DirectX::XMStoreFloat3(&newPos, _newPos);
			tf->SetTranslation(std::move(newPos));		
		}

		// Now do collision testing
		BroadPhaseCollisionDetection();
		MidPhaseCollisionDetection();
		NarrowPhaseCollisionDetection();
	}

	/// <summary>
	/// The broad phase of the collision system.
	/// </summary>
	void PhysicsSystem::BroadPhaseCollisionDetection() const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::Containers;
		using namespace CCE::ECS::Components;
		
		Octree<Entity> bspTree({20,20,20});

		// Add all collidable objects
		for (long long id : mEntities)
		{
			Entity e = Entity(id);
			BoxCollider* col = e.GetComponent<BoxCollider>();
		
			if (col != nullptr)
			{
				bspTree.Insert(e, e.GetComponent<Transform>()->Position(), { col->Width, col->Height, col->Length });
			}
		}
		
		// Insert all collision tuples into the global frame collisions
		bspTree.OperateOnContents(
			[&](std::vector<OctreeEntry<Entity>>& data) 
			{
				for (auto i = data.begin(); i != data.end(); ++i)
				{
					for (auto j = std::next(i); j != data.end(); ++j)
					{
						auto first = i->pObjRef->Id < j->pObjRef->Id ? i->pObjRef->Id : j->pObjRef->Id;
						auto second = i->pObjRef->Id > j->pObjRef->Id ? j->pObjRef->Id : i->pObjRef->Id;

						FrameCollisions.insert(std::move(Physics::CollisionInfo(first, second)));
					}
				}
			}
		);

		// @TODO: Multithread every cell (?)
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
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;

		for (auto& collision : FrameCollisions)
		{
			Entity first(collision.first);
			Entity second(collision.second);

			// Do not check for self collision
			DASSERT(first != second,
				"Checking for self collision is invalid! Both colliders have the same pointer!");
			
			if (CollideAABB(first.GetComponent<Transform>(), first.GetComponent<Collider>(),
				second.GetComponent<Transform>(), second.GetComponent<Collider>()))
			{
				/*e.GetComponent<Rigidbody>()->Reflect();
				other.GetComponent<Rigidbody>()->Reflect();*/

				first.GetComponent<Rigidbody>()->useGravity = false;
				second.GetComponent<Rigidbody>()->useGravity = false;
			}
		}		
	}

	std::vector<long long> PhysicsSystem::PhysicsWorld{};
	std::set<CCE::Physics::CollisionInfo> PhysicsSystem::FrameCollisions{};
	double PhysicsSystem::PhysicsCalcDuration = 0.0;
}

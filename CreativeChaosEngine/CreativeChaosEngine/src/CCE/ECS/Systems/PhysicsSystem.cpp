#include "PhysicsSystem.h"
#include "../Entity.h"
#include "../../Input/Keyboard.h"
#include "../../Analysis/Logger.h"
#include "../../Input/InputDevice.h"
#include "../EntityComponentSystem.h"
#include "../../Utilities/Math/CCMath.h"
#include "../Components/ComponentHeaders.h"
#include "../../Utilities/Containers/Octree.h"

namespace CCE::ECS::Systems
{
	using ECS = EntityComponentSystem;

	void PhysicsSystem::StartUp()
	{
		ECS::Instance->RegisterSystem<PhysicsSystem>();
		REGISTER_INPUT_CALLBACK;
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
		if (pause) { return; }	// Pause simulation

		auto start = Time::Now();
		Step();
		auto end = Time::Now();
		PhysicsCalcDuration = Time::GetDurationInMilliSec(start, end);
	}

	void PhysicsSystem::TogglePause()
	{
		pause = !pause;
	}

	void PhysicsSystem::Step()
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
				DirectX::XMVectorScale(XMLoadFloat3(&rb->velocity), Time::deltaTime));

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
	/// Constructs a spatial tree (octree) and inserts all objects in it.
	/// </summary>
	void PhysicsSystem::BroadPhaseCollisionDetection()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::Containers;
		using namespace CCE::ECS::Components;
		
		Octree<Entity> bspTree({5,5,5}, 15, 10);

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
					for (auto j = data.begin(); j != data.end(); ++j)
					{
						if (j->pObjRef.Id == i->pObjRef.Id) { continue; } // Disallow self collision!

						auto first = i->pObjRef.Id < j->pObjRef.Id ? i->pObjRef.Id : j->pObjRef.Id;
						auto second = i->pObjRef.Id < j->pObjRef.Id ? j->pObjRef.Id : i->pObjRef.Id;

						FrameCollisionCandidates.insert(Physics::CollisionInfo(first, second));
					}
				}
			}
		);

		// @TODO: Multithread every tree node (?)
	}

	/// <summary>
	/// The mid phase of the collision system.
	/// Checks for AABB collision of 
	/// </summary>
	void PhysicsSystem::MidPhaseCollisionDetection()
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;

		for (auto& collisionPair : FrameCollisionCandidates)
		{
			Entity first(collisionPair.first);
			Entity second(collisionPair.second);

			// Do not check for self collision
			DASSERT(first != second,
				"Checking for self collision is invalid! Both colliders have the same pointer!");

			if (CollideAABB(first.GetComponent<Transform>(), first.GetComponent<BoxCollider>(),
				second.GetComponent<Transform>(), second.GetComponent<BoxCollider>()))
			{
				FrameCollisions.insert(collisionPair);
			}
		}
	}

	/// <summary>
	/// The narrow phase of the collision system.
	/// Detects collisions and resolves them.
	/// </summary>
	void PhysicsSystem::NarrowPhaseCollisionDetection() const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		for (auto& collisionPair : FrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			Collider* colA = a.GetComponent<BoxCollider>();
			Collider* colB = b.GetComponent<BoxCollider>();

			CollisionInfo cInfo = collisionPair;

			if (!CollideInfoAABB(tfA, colA,tfB, colB, cInfo))
			{
				continue;
			}

			float totalInverseMass = 1.0f / (rbA->mass + rbB->mass);

			// Separating objects linearly
			XMFLOAT3 translationFirst{};
			XMFLOAT3 translationSecond{};

			XMStoreFloat3(&translationFirst, XMLoadFloat3(&tfA->Position()) - (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
				cInfo.contactPoint.penetration * (rbA->InverseMass() / totalInverseMass))));

			XMStoreFloat3(&translationSecond, XMLoadFloat3(&tfB->Position()) - (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
				cInfo.contactPoint.penetration * (rbB->InverseMass() / totalInverseMass))));

			tfA->SetTranslation(translationFirst);
			tfB->SetTranslation(translationSecond);

			// Set angular velocity and calc contact velocity

			XMVECTOR distAB = XMLoadFloat3(&tfB->Position()) - XMLoadFloat3(&tfA->Position());
			XMVECTOR distBA = XMLoadFloat3(&tfA->Position()) - XMLoadFloat3(&tfB->Position());

			// Calculate angular velocity
			XMVECTOR angVelA = XMVector3Cross(XMLoadFloat3(&rbA->angularVelocity), distAB);
			XMVECTOR angVelB = XMVector3Cross(XMLoadFloat3(&rbB->angularVelocity), distBA);

			XMVECTOR fullVelocityA = XMLoadFloat3(&rbA->velocity) + angVelA;
			XMVECTOR fullVelocityB = XMLoadFloat3(&rbB->velocity) + angVelB;

			XMVECTOR contactVelocity = fullVelocityB - fullVelocityA;

			XMFLOAT3 impulseForce{}; 
			XMStoreFloat3(&impulseForce, XMVector3Dot(contactVelocity, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

			// Calculate inertia
			XMVECTOR inertiaA = XMVector3Cross(XMLoadFloat3(&rbA->inertiaTensor) *
				XMVector3Cross(distAB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)), distAB);

			XMVECTOR inertiaB = XMVector3Cross(XMLoadFloat3(&rbB->inertiaTensor) *
				XMVector3Cross(distBA, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)), distBA);

			XMFLOAT3 angularEffect{};
			XMStoreFloat3(&angularEffect, XMVector3Dot(inertiaA + inertiaB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

			float damping = 0.33f;
			float j = (-(1.0f + damping) * impulseForce.x) / (totalInverseMass + angularEffect.x);

			XMVECTOR fullImpulse = XMLoadFloat3(&cInfo.contactPoint.collisionNormal) * j;

			// Apply impulses
			rbA->ApplyLinearImpulse(-fullImpulse);
			rbB->ApplyLinearImpulse(fullImpulse);
			
			rbA->ApplyAngularImpulse(XMVector3Cross(distAB, -fullImpulse));
			rbB->ApplyAngularImpulse(XMVector3Cross(distBA, fullImpulse));
		}
	}

	void PhysicsSystem::InputCallback(const Input::Mouse* mouse, const Input::Keyboard* keyboard, const Input::Controller* controller)
	{
		using namespace Input;
		if (keyboard->keys[(int)InputDevice::Keycode::SPACE] == Keyboard::ButtonState::PRESSED ||
			controller->RSouth == InputDevice::ButtonState::PRESSED)
		{
			TogglePause();
		}

	}

	std::vector<long long> PhysicsSystem::PhysicsWorld{};
	//std::set<CCE::Physics::CollisionInfo> PhysicsSystem::FrameCollisionCandidates{};
	//std::set<CCE::Physics::CollisionInfo> PhysicsSystem::FrameCollisions{};
	double PhysicsSystem::PhysicsCalcDuration = 0.0;
}

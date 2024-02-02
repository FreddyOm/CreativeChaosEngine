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
		
		Octree<Entity> bspTree({8,8,8}, 15, 15);
		DirectX::XMFLOAT3 boundingVolume;

		// Add all collidable objects
		for (long long id : mEntities)
		{
			Entity e = Entity(id);

			if(e.GetComponent<BoxCollider>() != nullptr)
			{
				auto* _col = e.GetComponent<BoxCollider>();
				bspTree.Insert(e, e.GetComponent<Transform>()->Position(), _col->GetBoundingBox());
			}
			else if (e.GetComponent<SphereCollider>() != nullptr)
			{
				auto* _col = e.GetComponent<SphereCollider>();
				bspTree.Insert(e, e.GetComponent<Transform>()->Position(), _col->GetBoundingBox());
			}
			else
			{
				DASSERT(false, "Invalid bounding volume type.");
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

			BoxCollider firstBox;
			BoxCollider secondBox;
			SphereCollider firstSphere;
			SphereCollider secondSphere;
			if (first.TryGetComponent<BoxCollider>(firstBox) && second.TryGetComponent<BoxCollider>(secondBox))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>(), first.GetComponent<BoxCollider>(),
					second.GetComponent<Transform>(), second.GetComponent<BoxCollider>()))
				{
					AABBAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<BoxCollider>(firstBox) && second.TryGetComponent<SphereCollider>(secondSphere))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>()->Position(), firstBox.GetBoundingBox(),
					second.GetComponent<Transform>()->Position(), secondSphere.GetBoundingBox()))
				{
					SphereAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<SphereCollider>(firstSphere) && second.TryGetComponent<BoxCollider>(secondBox))
			{
				// First AABB check
				if (CollideAABB(first.GetComponent<Transform>()->Position(), firstSphere.GetBoundingBox(),
					second.GetComponent<Transform>()->Position(), secondBox.GetBoundingBox()))
				{
					SphereAABBFrameCollisions.insert(collisionPair);
				}
			}
			else if (first.TryGetComponent<SphereCollider>(firstSphere) && second.TryGetComponent<SphereCollider>(secondSphere))
			{
				// First AABB check
				if (CollideSpheres(first.GetComponent<Transform>(), &firstSphere,
					second.GetComponent<Transform>(), &secondSphere))
				{
					SphereSphereFrameCollisions.insert(collisionPair);
				}
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

		// AABB-AABB-Collisions
		for (auto& collisionPair : AABBAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			BoxCollider* colBoxA = a.GetComponent<BoxCollider>();
			BoxCollider* colBoxB = b.GetComponent<BoxCollider>();

			CollisionInfo cInfo = collisionPair;

			if (!CollideInfoAABB(tfA, colBoxA, tfB, colBoxB, cInfo))
			{
				continue;
			}

			float totalInverseMass = rbA->InverseMass() + rbB->InverseMass();

			ApplyLinearTransformations(rbA, tfA, rbB, tfB, cInfo, totalInverseMass);

			// Impulse-based collision resolution
			ResolveCollisionImpulse(rbA, tfA, colBoxA, rbB, tfB, colBoxB, cInfo, totalInverseMass);

			ApplyAngularTransformations(rbA, tfA, rbB, tfB);
		}

		// Sphere-AABB-Collisions
		for (auto& collisionPair : SphereAABBFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			SphereCollider* colSphereA = a.GetComponent<SphereCollider>();
			SphereCollider* colSphereB = b.GetComponent<SphereCollider>();

			BoxCollider* colBoxA = a.GetComponent<BoxCollider>();
			BoxCollider* colBoxB = b.GetComponent<BoxCollider>();

			CollisionInfo cInfo = collisionPair;

			// Mixed collision
			auto* box = colBoxA == nullptr ? colBoxB : colBoxA;
			auto* sphere = colSphereA == nullptr ? colSphereB : colSphereA;

			if (!CollideInfoSphereAABB(tfB, box, tfA, sphere, cInfo))
			{
				continue;
			}

			float totalInverseMass = rbA->InverseMass() + rbB->InverseMass();

			ApplyLinearTransformations(rbB, tfB, rbA, tfA, cInfo, totalInverseMass);

			// Impulse-based collision resolution
			ResolveCollisionImpulse(rbB, tfB, box, rbA, tfA, sphere, cInfo, totalInverseMass);

			ApplyAngularTransformations(rbA, tfA, rbB, tfB);
		}

		// Sphere-Sphere-Collisions
		for (auto& collisionPair : SphereSphereFrameCollisions)
		{
			Entity a(collisionPair.first);
			Entity b(collisionPair.second);

			Rigidbody* rbA = a.GetComponent<Rigidbody>();
			Rigidbody* rbB = b.GetComponent<Rigidbody>();

			Transform* tfA = a.GetComponent<Transform>();
			Transform* tfB = b.GetComponent<Transform>();

			SphereCollider* colSphereA = a.GetComponent<SphereCollider>();
			SphereCollider* colSphereB = b.GetComponent<SphereCollider>();

			CollisionInfo cInfo = collisionPair;

			if (!CollideInfoSpheres(tfA, colSphereA, tfB, colSphereB, cInfo))
			{
				continue;
			}

			float totalInverseMass = rbA->InverseMass() + rbB->InverseMass();

			ApplyLinearTransformations(rbA, tfA, rbB, tfB, cInfo, totalInverseMass);

			// Impulse-based collision resolution
			ResolveCollisionImpulse(rbA, tfA, colSphereA, rbB, tfB, colSphereB, cInfo, totalInverseMass);

			ApplyAngularTransformations(rbA, tfA, rbB, tfB);
		}
	}

	void PhysicsSystem::ApplyAngularTransformations(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA, 
		CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;
		
		if (applyAngularImpulse)
		{
			XMFLOAT3 rotationFirst{};
			XMFLOAT3 rotationSecond{};

			XMStoreFloat3(&rotationFirst, XMVectorAdd(XMLoadFloat3(&tfA->Rotation()), XMLoadFloat3(&rbA->angularVelocity)));
			XMStoreFloat3(&rotationSecond, XMVectorAdd(XMLoadFloat3(&tfB->Rotation()), XMLoadFloat3(&rbB->angularVelocity)));

			tfA->SetRotation(rotationFirst);
			tfB->SetRotation(rotationSecond);
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

	void PhysicsSystem::ResolveCollisionImpulse(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA,
		CCE::ECS::Components::Collider* colA, CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB, 
		CCE::ECS::Components::Collider* colB, CCE::Physics::CollisionInfo& cInfo, float totalInverseMass) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Set angular velocity and calc contact velocity
		XMVECTOR relativeA =  XMLoadFloat3(&cInfo.contactPoint.collisionPointFirst) - XMLoadFloat3(&tfA->Position());
		XMVECTOR relativeB = XMLoadFloat3(&cInfo.contactPoint.collisionPointFirst) - XMLoadFloat3(&tfB->Position());

		// Calculate angular velocity
		XMVECTOR angVelA = XMVector3Cross(XMLoadFloat3(&rbA->angularVelocity), relativeA);
		XMVECTOR angVelB = XMVector3Cross(XMLoadFloat3(&rbB->angularVelocity), relativeB);

		XMVECTOR fullVelocityA = XMLoadFloat3(&rbA->velocity) + angVelA;
		XMVECTOR fullVelocityB = XMLoadFloat3(&rbB->velocity) + angVelB;

		XMVECTOR contactVelocity = fullVelocityB - fullVelocityA;

		XMFLOAT3 impulseForce{};
		XMStoreFloat3(&impulseForce, XMVector3Dot(contactVelocity, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

		// Calculate inertia
		XMVECTOR inertiaA = XMVector3Cross(XMVector3Transform(XMVector3Cross(relativeA, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)),
			XMLoadFloat3x3(&rbA->InertiaTensor(colA))), relativeA);

		XMVECTOR inertiaB = XMVector3Cross(XMVector3Transform(XMVector3Cross(relativeB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)),
			XMLoadFloat3x3(&rbB->InertiaTensor(colB))), relativeB);

		XMFLOAT3 angularEffect{};
		XMStoreFloat3(&angularEffect, XMVector3Dot(inertiaA + inertiaB, XMLoadFloat3(&cInfo.contactPoint.collisionNormal)));

		float damping = CCE::Math::CCMath::Clamp01((rbA->bounciness + rbB->bounciness) / 2.0f);
		float j = (-(1.0f + damping) * impulseForce.x) / (totalInverseMass + angularEffect.x);

		XMVECTOR fullImpulse = XMLoadFloat3(&cInfo.contactPoint.collisionNormal) * j;

		// Apply impulses
		rbA->ApplyLinearImpulse(-fullImpulse);
		rbB->ApplyLinearImpulse(fullImpulse);

		rbA->ApplyAngularImpulse(XMVector3Cross(relativeA, -fullImpulse));
		rbB->ApplyAngularImpulse(XMVector3Cross(relativeB, fullImpulse));
	}

	void PhysicsSystem::ApplyLinearTransformations(CCE::ECS::Components::Rigidbody* rbA, 
		CCE::ECS::Components::Transform* tfA, CCE::ECS::Components::Rigidbody* rbB, 
		CCE::ECS::Components::Transform* tfB, CCE::Physics::CollisionInfo& cInfo, float totalInverseMass) const
	{
		using Entity = CCE::ECS::Entity;
		using namespace CCE::ECS::Components;
		using namespace CCE::Physics;
		using namespace DirectX;

		// Separating objects linearly
		XMFLOAT3 translationFirst{};
		XMFLOAT3 translationSecond{};

		XMStoreFloat3(&translationFirst, XMLoadFloat3(&tfA->Position()) - (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
			cInfo.contactPoint.penetration * (rbA->InverseMass() / totalInverseMass))));

		XMStoreFloat3(&translationSecond, XMLoadFloat3(&tfB->Position()) + (XMVectorScale(XMLoadFloat3(&cInfo.contactPoint.collisionNormal),
			cInfo.contactPoint.penetration * (rbB->InverseMass() / totalInverseMass))));

		tfA->SetTranslation(translationFirst);
		tfB->SetTranslation(translationSecond);
	}

	std::vector<long long> PhysicsSystem::PhysicsWorld{};
	double PhysicsSystem::PhysicsCalcDuration = 0.0;
	bool PhysicsSystem::applyAngularImpulse = false;
}

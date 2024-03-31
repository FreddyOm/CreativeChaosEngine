/*
* Inspired by: https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf
*/

#pragma once
#include <DirectXMath.h>
#include "../ecs/components/component-headers.h"
#include <set>
#include "../analysis/debug.h"
#include "../analysis/logger.h"

namespace CCE::Physics
{
	struct ContactInfo
	{
		DirectX::XMFLOAT3 collisionPointFirst{};
		DirectX::XMFLOAT3 collisionPointSecond{};
		DirectX::XMFLOAT3 collisionNormal{};
		float penetration = 0;
	};

	/// <summary>
	/// A collision info struct to hold relevant data for the collision detection system.
	/// </summary>
	struct CollisionInfo
	{
		CollisionInfo(const long long _first, const long long _second)
			: first(_first), second(_second)
		{ }

		void SetContactPoint(const DirectX::XMFLOAT3& _pointFirst,
			const DirectX::XMFLOAT3& _pointSecond,
			const DirectX::XMFLOAT3& _normal, float _penetration)
		{
			contactPoint.collisionPointFirst = _pointFirst;
			contactPoint.collisionPointSecond = _pointSecond;
			contactPoint.collisionNormal = _normal;
			contactPoint.penetration = _penetration;
		}

		bool operator < (const CollisionInfo& other) const
		{
			size_t otherHash = (static_cast<size_t>(other.first) * 23) + (static_cast<size_t>(other.second) * 47);
			size_t thisHash = (static_cast<size_t>(first) * 23) + (static_cast<size_t>(second) * 47);

			return (thisHash < otherHash);
		}

		const long long first = -1;
		const long long second = -1;
		ContactInfo contactPoint{};
	};

	
	// @TODO: Rename meaningfully
	bool CollideAABB(const DirectX::XMFLOAT3& firstPosition, const DirectX::XMFLOAT3& firstSize, 
		const DirectX::XMFLOAT3& secondPosition, const DirectX::XMFLOAT3& secondSize);
	bool CollideAABB(CCE::ECS::Components::Transform* at, CCE::ECS::Components::Collider* ac, 
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::Collider* bc);
	bool CollideInfoAABB(const DirectX::XMFLOAT3& firstPosition, const DirectX::XMFLOAT3& firstSize,
		const DirectX::XMFLOAT3& secondPosition, const DirectX::XMFLOAT3& secondSize, CollisionInfo& collisionInfo);
	bool CollideInfoAABB(CCE::ECS::Components::Transform* at, CCE::ECS::Components::Collider* ac,
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::Collider* bc, CollisionInfo& collisionInfo);

	bool CollideSpheres(CCE::ECS::Components::Transform* at, CCE::ECS::Components::SphereCollider* ac,
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::SphereCollider* bc);
	bool CollideInfoSpheres(CCE::ECS::Components::Transform* at, CCE::ECS::Components::SphereCollider* ac,
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::SphereCollider* bc, CollisionInfo& collisionInfo);


	bool CollideInfoSphereAABB(CCE::ECS::Components::Transform* at, CCE::ECS::Components::BoxCollider* ac,
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::SphereCollider* bc, CollisionInfo& collisionInfo);

}

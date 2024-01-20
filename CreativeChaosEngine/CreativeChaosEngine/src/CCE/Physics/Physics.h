#pragma once
#include <DirectXMath.h>
#include "../ECS/Components/ComponentHeaders.h"

namespace CCE::Physics
{
	/// <summary>
	/// A collision info struct to hold relevant data for the collision detection system.
	/// </summary>
	struct CollisionInfo
	{
		CollisionInfo(const long long _first, const long long _second)
			: first(_first), second(_second)
		{ }

		bool operator < (const CollisionInfo& other) const
		{
			size_t otherHash = static_cast<size_t>(other.first) * (static_cast<size_t>(other.second) << 8);
			size_t thisHash = static_cast<size_t>(first) * (static_cast<size_t>(second) << 8);

			return (thisHash < otherHash);
		}

		const long long first = -1;
		const long long second = -1;
	};


	bool CollideAABB(const DirectX::XMFLOAT3& firstPosition, const DirectX::XMFLOAT3& firstSize, 
		const DirectX::XMFLOAT3& secondPosition, const DirectX::XMFLOAT3& secondSize);
	bool CollideAABB(CCE::ECS::Components::Transform* at, CCE::ECS::Components::Collider* ac, 
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::Collider* bc);
}

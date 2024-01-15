#pragma once
#include "../../Graphics/Rendering/D3D11.h"
#include "../Systems/PhysicsSystem.h"

namespace CCE::ECS::Components
{
	struct SphereCollider
	{
		float Radius = 1.0f;
		bool IsSolid = true;

		void Initialize(long long Id)
		{
			Systems::PhysicsSystem::collisionSystem.push_back(Id);
		}
	};

	struct BoxCollider
	{
		DirectX::XMFLOAT3 Center = { 0, 0, 0 };
		float Height = 1.f;
		float Width = 1.f;
		float Length = 1.f;
		bool IsSolid = true;

		void Initialize(long long Id)
		{
			Systems::PhysicsSystem::collisionSystem.push_back(Id);
		}
	};
}
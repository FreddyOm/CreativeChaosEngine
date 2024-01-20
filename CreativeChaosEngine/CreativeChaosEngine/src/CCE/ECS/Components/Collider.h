#pragma once
#include "../../Graphics/Rendering/D3D11.h"

namespace CCE::ECS::Components
{
	struct Collider
	{
		enum class Shape
		{
			Sphere,
			Box,
			Mesh
		};

		Shape Shape = Shape::Sphere;
	};

	struct SphereCollider : Collider
	{
		DirectX::XMFLOAT3 Center = { 0, 0, 0 };
		float Radius = 1.0f;
		bool IsSolid = true;

		void Initialize(long long Id);
	};

	struct BoxCollider : Collider
	{
		DirectX::XMFLOAT3 Center = { 0, 0, 0 };
		float Height = 1.f;
		float Width = 1.f;
		float Length = 1.f;
		bool IsSolid = true;

		void Initialize(long long Id);
	};	
}

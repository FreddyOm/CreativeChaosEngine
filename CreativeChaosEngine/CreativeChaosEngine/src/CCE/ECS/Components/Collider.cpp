#include "Collider.h"
#include "../../ECS/Systems/PhysicsSystem.h"

namespace CCE::ECS::Components
{
	void SphereCollider::Initialize(long long Id)
	{
		using namespace CCE::ECS::Systems;
		PhysicsSystem::PhysicsWorld.push_back(Id);
		Shape = Shape::Sphere;
	}

	DirectX::XMFLOAT3 SphereCollider::GetBoundingBox() const
	{
		return {Radius * 2, Radius * 2, Radius * 2};
	}


	void BoxCollider::Initialize(long long Id)
	{
		using namespace CCE::ECS::Systems;
		PhysicsSystem::PhysicsWorld.push_back(Id);
		Shape = Shape::Box;
	}

	DirectX::XMFLOAT3 BoxCollider::GetBoundingBox() const
	{
		return {Width, Height, Length};
	}

}
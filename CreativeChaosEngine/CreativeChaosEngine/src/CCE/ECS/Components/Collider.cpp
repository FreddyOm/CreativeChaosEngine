#include "Collider.h"
#include "../../ECS/Systems/PhysicsSystem.h"

namespace CCE::ECS::Components
{
	void SphereCollider::Initialize(long long Id)
	{
		OPTICK_EVENT();
		using namespace CCE::ECS::Systems;
		PhysicsSystem::PhysicsWorld.push_back(Id);
		Shape = Shape::Sphere;
	}

	DirectX::XMFLOAT3 SphereCollider::GetBoundingBox() const
	{
		OPTICK_EVENT();
		return {Radius * 2, Radius * 2, Radius * 2};
	}


	void BoxCollider::Initialize(long long Id)
	{
		OPTICK_EVENT();
		using namespace CCE::ECS::Systems;
		PhysicsSystem::PhysicsWorld.push_back(Id);
		Shape = Shape::Box;
	}

	DirectX::XMFLOAT3 BoxCollider::GetBoundingBox() const
	{
		OPTICK_EVENT();
		return {Width, Height, Length};
	}

}
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


	void BoxCollider::Initialize(long long Id)
	{
		using namespace CCE::ECS::Systems;
		PhysicsSystem::PhysicsWorld.push_back(Id);
		Shape = Shape::Box;
	}

}
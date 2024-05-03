#pragma once
#include <set>
#include <vector>
#include "ecs-system.h"
#include "../../core.h"
#include "../../analysis/time.h"
#include "../../physics/Physics.h"
#include "../../input/input.h"

namespace CCE::ECS::Systems
{
	struct CCE_API PhysicsSystem : public ECSSystem, protected Input::IInputHandler
	{
	public:
		PhysicsSystem() = default;
		~PhysicsSystem() = default;

		void StartUp();
		void ShutDown();

		void RegisterEntity(long long entity);

		void UpdateSystem();
		void TogglePause();

		static std::vector<long long> PhysicsWorld;
		std::set<Physics::CollisionInfo> FrameCollisionCandidates;
		std::set<CCE::Physics::CollisionInfo> AABBAABBFrameCollisions;
		std::set<CCE::Physics::CollisionInfo> SphereAABBFrameCollisions;
		std::set<CCE::Physics::CollisionInfo> SphereSphereFrameCollisions;
		static double PhysicsCalcDuration;
		static bool applyAngularImpulse;

	private:
		void Step();
		void BroadPhaseCollisionDetection();
		void MidPhaseCollisionDetection();
		void NarrowPhaseCollisionDetection() const;

		void ApplyAngularTransformations(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA,
			CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB) const;

		void ApplyLinearTransformations(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA,
			CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB, CCE::Physics::CollisionInfo& cInfo,
			float totalInverseMass) const;

		void ResolveCollisionImpulse(CCE::ECS::Components::Rigidbody* rbA, CCE::ECS::Components::Transform* tfA,
			CCE::ECS::Components::Collider* colA, CCE::ECS::Components::Rigidbody* rbB, CCE::ECS::Components::Transform* tfB, 
			CCE::ECS::Components::Collider* colB, CCE::Physics::CollisionInfo& cInfo, float totalInverseMass) const;

		// Inherited via IInputHandler
		void InputCallback(const Input::Mouse* mouse,
			const Input::Keyboard* keyboard,
			const Input::Controller* controller) override;

	private:
		std::chrono::steady_clock::time_point start{};
		std::chrono::steady_clock::time_point end{};
		bool pause = false;
	};
}

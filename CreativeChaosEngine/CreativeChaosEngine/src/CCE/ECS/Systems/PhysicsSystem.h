#pragma once
#include "ECSSystem.h"
#include "../../Core.h"
#include "../../Analysis/Time.h"
#include "../../Physics/Physics.h"
#include <vector>
#include <set>

namespace CCE::ECS::Systems
{
	struct CCE_API PhysicsSystem : public ECSSystem
	{
	public:
		PhysicsSystem() = default;
		~PhysicsSystem() = default;

		void StartUp();
		void ShutDown();

		void RegisterEntity(long long entity);

		void UpdateSystem();

		static std::vector<long long> PhysicsWorld;
		static std::set<Physics::CollisionInfo> FrameCollisions;
		static double PhysicsCalcDuration;

	private:
		void UpdateECSBasic();
		void BroadPhaseCollisionDetection() const;
		void MidPhaseCollisionDetection() const;
		void NarrowPhaseCollisionDetection() const;
	};
}

#pragma once
#include "ECSSystem.h"
#include "../../Core.h"
#include <vector>
#include "../../Analysis/Time.h"

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

		static std::vector<long long> CollisionSystem;
		static double PhysicsCalcDuration;

	private:
		void UpdateECSBasic();
		void BroadPhaseCollisionDetection() const;
		void MidPhaseCollisionDetection() const;
		void NarrowPhaseCollisionDetection(long long id) const;
	};
}

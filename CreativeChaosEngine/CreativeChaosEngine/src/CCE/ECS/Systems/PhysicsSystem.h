#pragma once
#include "ECSSystem.h"
#include <vector>

namespace CCE::ECS::Systems
{
	struct PhysicsSystem : public ECSSystem
	{
	public:
		PhysicsSystem() = default;
		~PhysicsSystem() = default;

		void StartUp();
		void ShutDown();

		void RegisterEntity(long long entity);

		void UpdateSystem();

		static std::vector<long long> collisionSystem;

	private:
		void UpdateECSBasic();
		void BroadPhaseCollisionDetection() const;
		void MidPhaseCollisionDetection() const;
		void NarrowPhaseCollisionDetection() const;
	};
}

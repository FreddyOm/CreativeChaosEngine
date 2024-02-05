#pragma once
#include <set>
#include <vector>
#include "ECSSystem.h"
#include "../../Core.h"
#include "../../Analysis/Time.h"
#include "../../Physics/Physics.h"
#include "../../Manager/InputManager.h"
#include "../../ComputeShader/ComputeShader.h"
#include <wrl.h>

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

		struct CollisionPairInData
		{
			DirectX::XMFLOAT4 positionA{};
			DirectX::XMFLOAT4 positionB{};
			DirectX::XMFLOAT4 colliderDimA{};
			DirectX::XMFLOAT4 colliderDimB{};
			DirectX::XMFLOAT4 velocityA{};
			DirectX::XMFLOAT4 velocityB{};
			DirectX::XMFLOAT4 angularVelocityA{};
			DirectX::XMFLOAT4 angularVelocityB{};
			DirectX::XMFLOAT4 bouncinessA{};
			DirectX::XMFLOAT4 bouncinessB{};
			DirectX::XMFLOAT4 massA{};
			DirectX::XMFLOAT4 massB{};
			DirectX::XMFLOAT4 shapeA{};
			DirectX::XMFLOAT4 shapeB{};

			DirectX::XMFLOAT4 collisionPointA{};
			DirectX::XMFLOAT4 collisionPointB{};
			DirectX::XMFLOAT4 collisionNormal{};
			DirectX::XMFLOAT4 penetration{};

			char padding[224];
		};

		struct CSInputData
		{
			std::vector<CollisionPairInData> cpd{};
		};

		struct CollisionPairOutData
		{
			DirectX::XMFLOAT4 newPositionA{};
			DirectX::XMFLOAT4 newPositionB{};
			DirectX::XMFLOAT4 linearImpulseA{};
			DirectX::XMFLOAT4 linearImpulseB{};
			DirectX::XMFLOAT4 angularImpulseA{};
			DirectX::XMFLOAT4 angularImpulseB{};

			char padding[32];
		};

		struct CSOutputData
		{
			std::vector<CollisionPairOutData> cpd{};
		};

	private:
		void Step();
		void BroadPhaseCollisionDetection();
		void MidPhaseCollisionDetection();
		void NarrowPhaseCollisionDetection() const;

		void FillConstantBuffer(CCE::ECS::Systems::PhysicsSystem::CSInputData& inData) const;

		void ApplyComputeShaderData(CCE::ECS::Systems::PhysicsSystem::CSOutputData* dataView) const;

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
		mutable Microsoft::WRL::ComPtr<ID3D11ComputeShader> pComputeShader{};
		std::chrono::steady_clock::time_point start{};
		std::chrono::steady_clock::time_point end{};
		bool pause = false;
	};
}

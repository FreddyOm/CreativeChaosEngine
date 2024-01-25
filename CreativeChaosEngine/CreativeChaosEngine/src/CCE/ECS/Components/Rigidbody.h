#pragma once
#include "../../Graphics/Rendering/D3D11.h"
#include "Collider.h"

namespace CCE::ECS::Components
{
	constexpr float GravitationalAccelarion()
	{
		return 9.81f;
	}

	class Rigidbody
	{
	public:

		Rigidbody() : mass(1), useGravity(true) {}
		
		Rigidbody(double mass)
		: mass(mass)
		{}

		Rigidbody(bool useGravity, double mass)
			: useGravity(useGravity), mass(mass)
		{}
		
		~Rigidbody() {}

	public:

		void ApplyLinearImpulse(const DirectX::XMVECTOR& force);
		void ApplyAngularImpulse(const DirectX::XMVECTOR& force);

		void UpdateRigidbody();
		constexpr float InverseMass() 
		{
			return mass <= 0.000001f && mass >= -0.000001f ? 0.0f : 1.0f / mass;
		}

		DirectX::XMFLOAT3X3& InertiaTensor(Collider* collider);

		DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 acceleration = { 0.0f, 0.0f, 0.0f };

		DirectX::XMFLOAT3 angularVelocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3X3* inertiaTensor = nullptr;
		
	public:

		bool useGravity = false;
		float mass = 1; 
		float bounciness = 0.25f;
		// Shape and stuff ...

	private:
		
		// Angular dynamics
		DirectX::XMFLOAT3 omega = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 angularMomentum = {};
	};
}

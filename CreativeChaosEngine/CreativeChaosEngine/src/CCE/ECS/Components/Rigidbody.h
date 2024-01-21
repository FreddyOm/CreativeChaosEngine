#pragma once
#include "../../Graphics/Rendering/D3D11.h"

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
			return mass <= 0.000001 && mass >= -0.000001 ? 0.0f : 1.0f / mass;
		}

		DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 acceleration = { 0.0f, 0.0f, 0.0f };

		DirectX::XMFLOAT3 angularVelocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 inertiaTensor = { 0.0f, 0.0f, 0.0f };

	public:

		bool useGravity = false;
		double mass = 1; 
		// Shape and stuff ...

	private:
		
		// Angular dynamics
		DirectX::XMFLOAT3 omega = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 angularMomentum = {};
	};
}

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

		Rigidbody() {}
		
		Rigidbody(double mass)
		: mass(mass)
		{}

		Rigidbody(bool useGravity, double mass)
			: useGravity(useGravity), mass(mass)
		{}
		
		~Rigidbody() {}

	public:

		void AddForce(DirectX::XMVECTOR force);

	public:

		bool useGravity = false;
		double mass = 1; 
		// Shape and stuff ...

	private:

		DirectX::XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT3 acceleration = { 0.0f, 0.0f, 0.0f };
	};
}

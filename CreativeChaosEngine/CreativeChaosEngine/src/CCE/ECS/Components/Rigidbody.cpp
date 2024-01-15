#include "Rigidbody.h"
#include "../../Analysis/Debug.h"
#include "../../Analysis/Time.h"

namespace CCE::ECS::Components
{
	void Rigidbody::AddForce(DirectX::XMVECTOR force)
	{
		using namespace DirectX;

		DASSERT(0 < mass,
			"Cannot add force to object with mass less or equal to 0!");

		XMVECTOR newAcc = XMLoadFloat3(&acceleration) + XMVectorScale(force, 1.0 / mass);
		
		XMStoreFloat3(&acceleration, XMVectorScale(newAcc, Time::deltaTime));
		XMStoreFloat3(&velocity, XMLoadFloat3(&velocity) + newAcc);
	}

	void Rigidbody::UpdateRigidbody()
	{
		using namespace DirectX;
		if (!useGravity) { return; }
		
		XMVECTOR force = { 0, -GravitationalAccelarion(), 0 };
		XMVECTOR vel = XMLoadFloat3(&velocity);
		// Do gravity
		XMStoreFloat3(&acceleration, XMVectorScale(force, Time::deltaTime));
		XMStoreFloat3(&velocity, vel + XMLoadFloat3(&acceleration));
	}

	DirectX::XMVECTOR Rigidbody::Velocity() const
	{
		return DirectX::XMLoadFloat3(&velocity);
	}

	DirectX::XMVECTOR Rigidbody::Acceleration() const
	{
		return DirectX::XMLoadFloat3(&acceleration);
	}
}
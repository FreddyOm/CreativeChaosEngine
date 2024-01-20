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
		
		if (useGravity)
		{
			// Do gravity
			XMVECTOR force = { 0, -GravitationalAccelarion() / 1000, 0 };
			XMStoreFloat3(&acceleration, XMVectorAdd(XMLoadFloat3(&acceleration), XMVectorScale(force, Time::deltaTime)));
		}
		else 
		{
			XMStoreFloat3(&velocity, { 0,0,0 });
			XMStoreFloat3(&acceleration, {0,0,0});
		}
				
		XMStoreFloat3(&velocity, XMLoadFloat3(&velocity) + XMLoadFloat3(&acceleration));
	}

	void Rigidbody::Reflect(const float frictionCoeff)
	{
		// @TODO: Reflect the velocity in order to make the object bounce. Later rflect on collision normal!
		using namespace DirectX;
		XMStoreFloat3(&velocity, XMVectorScale({ velocity.x, -velocity.y, velocity.z }, frictionCoeff));
		XMStoreFloat3(&acceleration, XMVectorScale({ acceleration.x, -acceleration.y, acceleration.z }, frictionCoeff));
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
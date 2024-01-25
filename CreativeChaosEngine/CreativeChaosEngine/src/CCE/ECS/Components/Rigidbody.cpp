#include "Rigidbody.h"
#include "../../Analysis/Debug.h"
#include "../../Analysis/Time.h"

namespace CCE::ECS::Components
{
	/// <summary>
	/// Adds a force to the rigidbody object.
	/// </summary>
	/// <param name="force">The force to apply to the rigidbody.</param>
	void Rigidbody::ApplyLinearImpulse(const DirectX::XMVECTOR& force)
	{
		using namespace DirectX;

		if (mass == 0) { return; }

		XMVECTOR newAcc = XMLoadFloat3(&velocity) + XMVectorScale(force, InverseMass());
		XMStoreFloat3(&velocity, newAcc);
	}


	void Rigidbody::ApplyAngularImpulse(const DirectX::XMVECTOR& force)
	{
		using namespace DirectX;

		XMVECTOR inverseForce = XMVectorDivide({1.0f, 1.0f, 1.0f}, force);
		XMVECTOR scaledInertiaTensor = XMVector3Transform(force, XMLoadFloat3x3(&inertiaTensor));
		XMVECTOR newAngVel = XMVectorAdd(XMLoadFloat3(&angularVelocity), scaledInertiaTensor);

		XMStoreFloat3(&angularVelocity, newAngVel);
	}

	/// <summary>
	/// Updates the 
	/// </summary>
	void Rigidbody::UpdateRigidbody()
	{
		using namespace DirectX;
		
		if (useGravity)
		{
			// Do gravity
			XMVECTOR force = { 0, -GravitationalAccelarion() / 1000, 0 };
			ApplyLinearImpulse(force);
		}
	}
}

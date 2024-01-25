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

		XMVECTOR inverseForce = XMVectorDivide({1.0f, 1.0f, 1.0f}, force / 1000.f);
		XMVECTOR scaledInertiaTensor = XMVector3Transform(force, XMLoadFloat3x3(inertiaTensor));
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

	/// <summary>
	/// Returns the inertia tensor of this rigidbody for a given collider type.
	/// </summary>
	/// <param name="collider">The collider of this object.</param>
	/// <returns>A matrix containing the inertia coefficients for each rotational axis.</returns>
	DirectX::XMFLOAT3X3& Rigidbody::InertiaTensor(Collider* collider)
	{
		if (inertiaTensor == nullptr)
		{
			inertiaTensor = new DirectX::XMFLOAT3X3();
			SphereCollider* colSphere = nullptr;
			BoxCollider* colBox = nullptr;

			float I = 0;
			float Ix = 0;
			float Iy = 0;
			float Iz = 0;

			switch (collider->Shape)
			{

			case Collider::Shape::Sphere:
				colSphere = reinterpret_cast<SphereCollider*>(collider);
				I = (2.0f / 5.0f) * mass * (colSphere->Radius * colSphere->Radius);
				*inertiaTensor = {
				{I} ,{} ,{},
				{} ,{I} ,{},
				{} ,{} ,{I}
				};
				break;
			case Collider::Shape::Box:
				colBox = reinterpret_cast<BoxCollider*>(collider);

				Ix = (1.0f / 12.0f) * mass * (colBox->Height * colBox->Height + colBox->Length * colBox->Length);
				Iy = (1.0f / 12.0f) * mass * (colBox->Width * colBox->Width + colBox->Length * colBox->Length);
				Iz = (1.0f / 12.0f) * mass * (colBox->Height * colBox->Height + colBox->Width * colBox->Width);

				*inertiaTensor = {
				{Ix} ,{} ,{},
				{} ,{Iy} ,{},
				{} ,{} ,{Iz}
				};
				break;
			case Collider::Shape::Mesh:
				DASSERT(false, "Inertia tensor not yet supported for mesh collider!");
				break;
			}
		}
		
		return *inertiaTensor;
	}
}

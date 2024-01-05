#include "Rigidbody.h"
#include "../../Analysis/Debug.h"

namespace CCE::ECS::Components
{
	void Rigidbody::AddForce(DirectX::XMVECTOR force)
	{
		using namespace DirectX;
		// F = m * a
		// a = F / m

		// TODO: How long do we add the force? Just once I guess... 
		// ... but that means I have to regulate when to add and when not...

		DASSERT(0 >= mass,
			"Cannot add force to object with mass less or equal to 0!");

		XMVECTOR newAcc = XMLoadFloat3(&acceleration) + XMVectorScale(force, 1.0 / mass);
		XMStoreFloat3(&acceleration, newAcc);
	}
}
#include "Physics.h"
#include "../Analysis/Debug.h"

namespace CCE::Physics
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="firstPosition"></param>
	/// <param name="firstSize"></param>
	/// <param name="secondPosition"></param>
	/// <param name="secondSize"></param>
	/// <returns></returns>
	bool CollideAABB(const DirectX::XMFLOAT3& firstPosition, const DirectX::XMFLOAT3& firstSize, 
		const DirectX::XMFLOAT3& secondPosition, const DirectX::XMFLOAT3& secondSize)
	{
		return firstPosition.x - (firstSize.x) <=
			secondPosition.x + (secondSize.x) &&		// first.minX <= second.maxX
			firstPosition.x + (firstSize.x) >=
			secondPosition.x - (secondSize.x) &&		// first.maxX >= second.minX
			firstPosition.y - (firstSize.y) <=
			secondPosition.y + (secondSize.y) &&		// first.minY <= second.maxY
			firstPosition.y + (firstSize.y) >=
			secondPosition.y - (secondSize.y) &&		// first.maxY >= second.minY
			firstPosition.z - (firstSize.z) <=
			secondPosition.z + (secondSize.z) &&		// first.minZ <= second.maxZ
			firstPosition.z + (firstSize.z) >=
			secondPosition.z - (secondSize.z);			// first.maxZ >= second.minZ
	}

	/// <summary>
	/// Checks if there is a collision of two AABBs.
	/// </summary>
	/// <param name="firstTransform">The transform of the first object.</param>
	/// <param name="firstCollider">The collider of the first object.</param>
	/// <param name="secondTransform">The transform of the second object.</param>
	/// <param name="secondCollider">The collider of the second object.</param>
	/// <returns>True if collision was registered, false if not.</returns>
	bool CollideAABB(ECS::Components::Transform* firstTransform, ECS::Components::Collider* firstCollider,
		ECS::Components::Transform* secondTransform, ECS::Components::Collider* secondCollider)
	{
		DASSERT(firstCollider->Shape == ECS::Components::Collider::Shape::Box &&
			secondCollider->Shape == ECS::Components::Collider::Shape::Box,
			"Cannot use any other bounding shape other than a box for AABB test.");

		ECS::Components::BoxCollider* fc = reinterpret_cast<ECS::Components::BoxCollider*>(firstCollider);
		ECS::Components::BoxCollider* sc = reinterpret_cast<ECS::Components::BoxCollider*>(secondCollider);

		return firstTransform->Position().x - (fc->Width) <=
			secondTransform->Position().x + (sc->Width) &&		// first.minX <= second.maxX
			firstTransform->Position().x + (fc->Width) >=
			secondTransform->Position().x - (sc->Width) &&		// first.maxX >= second.minX
			firstTransform->Position().y - (fc->Height) <=
			secondTransform->Position().y + (sc->Height) &&		// first.minY <= second.maxY
			firstTransform->Position().y + (fc->Height) >=
			secondTransform->Position().y - (sc->Height) &&		// first.maxY >= second.minY
			firstTransform->Position().z - (fc->Length) <=
			secondTransform->Position().z + (sc->Length) &&		// first.minZ <= second.maxZ
			firstTransform->Position().z + (fc->Length) >=
			secondTransform->Position().z - (sc->Length);			// first.maxZ >= second.minZ
	}
}

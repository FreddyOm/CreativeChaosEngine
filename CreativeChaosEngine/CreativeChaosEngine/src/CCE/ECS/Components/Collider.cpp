#include "Collider.h"

namespace CCE::ECS::Components{

	/// <summary>
	/// Checks if there is a collision of two AABBs.
	/// </summary>
	/// <param name="firstTransform">The transform of the first object.</param>
	/// <param name="firstCollider">The collider of the first object.</param>
	/// <param name="secondTransform">The transform of the second object.</param>
	/// <param name="secondCollider">The collider of the second object.</param>
	/// <returns></returns>
	bool CollideAABB(Transform& firstTransform, BoxCollider& firstCollider, Transform& secondTransform, BoxCollider& secondCollider)
	{
		return firstTransform.Position().x - (firstCollider.Width) <= 
			secondTransform.Position().x + (secondCollider.Width)   &&		// first.minX <= second.maxX
			   firstTransform.Position().x + (firstCollider.Width ) >= 
			secondTransform.Position().x - (secondCollider.Width)  &&		// first.maxX >= second.minX
			   firstTransform.Position().y - (firstCollider.Height) <= 
			secondTransform.Position().y + (secondCollider.Height) &&		// first.minY <= second.maxY
			   firstTransform.Position().y + (firstCollider.Height) >= 
			secondTransform.Position().y - (secondCollider.Height) &&		// first.maxY >= second.minY
			   firstTransform.Position().z - (firstCollider.Length) <= 
			secondTransform.Position().z + (secondCollider.Length) &&		// first.minZ <= second.maxZ
			   firstTransform.Position().z + (firstCollider.Length) >= 
			secondTransform.Position().z - (secondCollider.Length);			// first.maxZ >= second.minZ
	}
}


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
	bool CollideAABB(CCE::ECS::Components::Transform* firstTransform, CCE::ECS::Components::Collider* firstCollider,
		CCE::ECS::Components::Transform* secondTransform, CCE::ECS::Components::Collider* secondCollider)
	{
		using namespace CCE::ECS::Components;

		DASSERT(firstCollider->Shape == ECS::Components::Collider::Shape::Box &&
			secondCollider->Shape == ECS::Components::Collider::Shape::Box,
			"Cannot use any other bounding shape other than a box for AABB test.");

		DirectX::XMFLOAT3 fc;
		DirectX::XMFLOAT3 sc;

		switch (firstCollider->Shape)
		{
		case Collider::Shape::Box:
			fc = reinterpret_cast<ECS::Components::BoxCollider*>(firstCollider)->GetBoundingBox();
			break;
		case Collider::Shape::Sphere:
			fc = reinterpret_cast<ECS::Components::SphereCollider*>(firstCollider)->GetBoundingBox();
			break;
		default:
			DERROR("Can't compute collider bounding boyes other than box and sphere!");
			break;
		}

		switch (secondCollider->Shape)
		{
		case Collider::Shape::Box:
			sc = reinterpret_cast<ECS::Components::BoxCollider*>(secondCollider)->GetBoundingBox();
			break;
		case Collider::Shape::Sphere:
			sc = reinterpret_cast<ECS::Components::SphereCollider*>(secondCollider)->GetBoundingBox();
			break;
		default:
			DERROR("Can't compute collider bounding boyes other than box and sphere!");
			break;
		}

		return firstTransform->Position().x - (fc.x) <=
			secondTransform->Position().x + (sc.x) &&		// first.minX <= second.maxX
			firstTransform->Position().x + (fc.x) >=
			secondTransform->Position().x - (sc.x) &&		// first.maxX >= second.minX
			firstTransform->Position().y - (fc.y) <=
			secondTransform->Position().y + (sc.y) &&		// first.minY <= second.maxY
			firstTransform->Position().y + (fc.y) >=
			secondTransform->Position().y - (sc.y) &&		// first.maxY >= second.minY
			firstTransform->Position().z - (fc.z) <=
			secondTransform->Position().z + (sc.z) &&		// first.minZ <= second.maxZ
			firstTransform->Position().z + (fc.z) >=
			secondTransform->Position().z - (sc.z);			// first.maxZ >= second.minZ
	}

	bool CollideInfoAABB(const DirectX::XMFLOAT3& firstPosition, const DirectX::XMFLOAT3& firstSize, 
		const DirectX::XMFLOAT3& secondPosition, const DirectX::XMFLOAT3& secondSize, CollisionInfo& collisionInfo)
	{
		using namespace DirectX;

		bool overlap = CollideAABB(firstPosition, firstSize, secondPosition, secondSize);

		if (overlap)
		{
			static const XMFLOAT3 faces[6] =
			{
				{ -1, 0, 0 }
				,{ 1, 0, 0 }
				,{ 0, -1, 0 }
				,{ 0, 1, 0 }
				,{ 0, 0, -1 }
				,{ 0, 0, 1 },
			};

			XMFLOAT3 maxFirst{}; XMStoreFloat3(&maxFirst, XMLoadFloat3(&firstPosition) + XMLoadFloat3(&firstSize));
			XMFLOAT3 minFirst{}; XMStoreFloat3(&minFirst, XMLoadFloat3(&firstPosition) - XMLoadFloat3(&firstSize));
			
			XMFLOAT3 maxSecond{}; XMStoreFloat3(&maxSecond, XMLoadFloat3(&secondPosition) + XMLoadFloat3(&secondSize));
			XMFLOAT3 minSecond{}; XMStoreFloat3(&minSecond, XMLoadFloat3(&secondPosition) - XMLoadFloat3(&secondSize));

			float distances[6] =
			{
				(maxSecond.x - minFirst.x),		// distance of box ’b’ to ’left ’ of ’a ’.
				(maxFirst.x - minSecond.x),		// distance of box ’b’ to ’right ’ of ’a ’.
				(maxSecond.y - minFirst.y),		// distance of box ’b’ to ’bottom ’ of ’a ’.
				(maxFirst.y - minSecond.y),		// distance of box ’b’ to ’top ’ of ’a ’.
				(maxSecond.z - minFirst.z),		// distance of box ’b’ to ’far ’ of ’a ’.
				(maxFirst.z - minSecond.z)		// distance of box ’b’ to ’near ’ of ’a ’.
			};

			float penetration = FLT_MAX;
			XMFLOAT3 normal{};

			for (int i = 0; i < 6; ++i)
			{
				if (distances[i] < penetration)
				{
					penetration = distances[i];
					normal = faces[i];
				}
			}
			collisionInfo.SetContactPoint({0,0,0}, {0,0,0}, std::move(normal), std::move(penetration));
			return true;
		}
		return false;
	}

	bool CollideInfoAABB(CCE::ECS::Components::Transform* at, CCE::ECS::Components::Collider* ac, 
		CCE::ECS::Components::Transform* bt, CCE::ECS::Components::Collider* bc, CollisionInfo& collisionInfo)
	{
		using namespace DirectX;
		using namespace CCE::ECS::Components;

		bool overlap = CollideAABB(at, ac, bt, bc);

		BoxCollider* colA = reinterpret_cast<BoxCollider*>(ac);
		BoxCollider* colB = reinterpret_cast<BoxCollider*>(bc);

		if (overlap)
		{
			static const XMFLOAT3 faces[6] =
			{
				{ -1, 0, 0 }
				,{ 1, 0, 0 }
				,{ 0, -1, 0 }
				,{ 0, 1, 0 }
				,{ 0, 0, -1 }
				,{ 0, 0, 1 },
			};

			XMFLOAT3 maxFirst{}; XMStoreFloat3(&maxFirst, XMLoadFloat3(&at->Position()) + XMVECTOR{ colA->Width,  colA->Height, colA->Length});
			XMFLOAT3 minFirst{}; XMStoreFloat3(&minFirst, XMLoadFloat3(&at->Position()) - XMVECTOR{ colA->Width,  colA->Height, colA->Length });

			XMFLOAT3 maxSecond{}; XMStoreFloat3(&maxSecond, XMLoadFloat3(&bt->Position()) + XMVECTOR{ colB->Width,  colB->Height, colB->Length });
			XMFLOAT3 minSecond{}; XMStoreFloat3(&minSecond, XMLoadFloat3(&bt->Position()) - XMVECTOR{ colB->Width,  colB->Height, colB->Length });

			float distances[6] =
			{
				(maxSecond.x - minFirst.x),		// distance of box ’b’ to ’left ’ of ’a ’.
				(maxFirst.x - minSecond.x),		// distance of box ’b’ to ’right ’ of ’a ’.
				(maxSecond.y - minFirst.y),		// distance of box ’b’ to ’bottom ’ of ’a ’.
				(maxFirst.y - minSecond.y),		// distance of box ’b’ to ’top ’ of ’a ’.
				(maxSecond.z - minFirst.z),		// distance of box ’b’ to ’far ’ of ’a ’.
				(maxFirst.z - minSecond.z)		// distance of box ’b’ to ’near ’ of ’a ’.
			};

			float penetration = FLT_MAX;
			XMFLOAT3 normal{};

			for (int i = 0; i < 6; ++i)
			{
				if (distances[i] < penetration)
				{
					penetration = distances[i];
					normal = faces[i];
				}
			}
			collisionInfo.SetContactPoint({ 0,0,0 }, { 0,0,0 }, normal, penetration);
			return true;
		}
		return false;
	}

	/// <summary>
	/// Checks if two spheres are touching or intersecting.
	/// </summary>
	/// <param name="at">The first transform.</param>
	/// <param name="ac">The first collider.</param>
	/// <param name="bt">The second transform.</param>
	/// <param name="bc">The second collider.</param>
	/// <returns>True, if spheres are touching/intersecting, false if not.</returns>
	bool CollideSpheres(CCE::ECS::Components::Transform* at, CCE::ECS::Components::SphereCollider* ac, CCE::ECS::Components::Transform* bt, CCE::ECS::Components::SphereCollider* bc)
	{
		using namespace DirectX;
		
		XMFLOAT3 vecAB; XMStoreFloat3(&vecAB, XMLoadFloat3(&bt->Position()) - XMLoadFloat3(&at->Position()));
		float sqrdDist = (vecAB.x * vecAB.x) + (vecAB.y * vecAB.y) + (vecAB.y * vecAB.y);
		float radiiSum = ac->Radius + bc->Radius;
		float sqrdRadiiSum = radiiSum * radiiSum;

		return sqrdDist <= sqrdRadiiSum;
	}

	bool CollideInfoSpheres(CCE::ECS::Components::Transform* at, CCE::ECS::Components::SphereCollider* ac, CCE::ECS::Components::Transform* bt, CCE::ECS::Components::SphereCollider* bc, CollisionInfo& collisionInfo)
	{
		using namespace DirectX;
		using namespace CCE::ECS::Components;

		XMFLOAT3 vecAB; XMStoreFloat3(&vecAB, XMLoadFloat3(&bt->Position()) - XMLoadFloat3(&at->Position()));
		float sqrdDist = (vecAB.x * vecAB.x) + (vecAB.y * vecAB.y) + (vecAB.y * vecAB.y);
		float radiiSum = ac->Radius + bc->Radius;
		float sqrdRadiiSum = radiiSum * radiiSum;

		bool overlap = sqrdDist <= sqrdRadiiSum;

		if (overlap)
		{
			float penetration = sqrtf(sqrdRadiiSum) - sqrtf(sqrdDist);
			XMFLOAT3 normal; XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&vecAB)));
			
			XMVECTOR scaledVecFirst = XMVectorScale(XMLoadFloat3(&vecAB), ac->Radius);
			XMVECTOR scaledVecSecond = XMVectorScale(XMLoadFloat3(&vecAB), -ac->Radius);

			XMFLOAT3 firstContactPoint; XMStoreFloat3(&firstContactPoint, XMVectorAdd(XMLoadFloat3(&at->Position()), scaledVecFirst));
			XMFLOAT3 secondContactPoint; XMStoreFloat3(&secondContactPoint, XMVectorAdd(XMLoadFloat3(&bt->Position()), scaledVecSecond));

			collisionInfo.SetContactPoint(firstContactPoint, secondContactPoint, normal, penetration);
			return true;
		}

		return false;
	}

	void ResolveCollision(std::set<CollisionInfo>& collisions, int maxIterations)
	{

	}

}

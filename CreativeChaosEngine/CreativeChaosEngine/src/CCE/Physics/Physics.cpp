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

	void ResolveCollision(std::set<CollisionInfo>& collisions, int maxIterations)
	{

	}

}

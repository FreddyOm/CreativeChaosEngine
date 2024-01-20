/*
* Inspired by this paper:
* https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/6accelerationstructures/Physics%20-%20Spatial%20Acceleration%20Structures.pdf
*/

#pragma once
#include "../../Graphics/Rendering/D3D11.h"
#include <functional>
#include <vector>
#include "../../ECS/Entity.h"
#include "../../Physics/Physics.h"

namespace CCE::Containers
{
	template<typename T>
	class Octree;

	template<typename T>
	struct OctreeEntry
	{
		OctreeEntry(const T* _pObjRef, const DirectX::XMFLOAT3 _pos, const DirectX::XMFLOAT3 _size)
			: pObjRef(_pObjRef), pos(_pos), size(_size)
		{ }

		OctreeEntry(const T* _pObjRef, const DirectX::XMVECTOR& _pos, const DirectX::XMVECTOR& _size)
			: pObjRef(_pObjRef)
		{ 
			DirectX::XMStoreFloat3(&pos, _pos);
			DirectX::XMStoreFloat3(&size, _size);
		}

		const T* pObjRef = nullptr;
		DirectX::XMFLOAT3 pos{};
		DirectX::XMFLOAT3 size{};
	};

	template<typename T>
	class OctreeNode
	{
	public:
		typedef std::function<void(std::vector<OctreeEntry<T>>&)> OctreeCallback;

		/// <summary>
		/// Calls the callback function on the children of the tree or, if none
		/// exist, in its content.
		/// </summary>
		/// <typeparam name="T">The data type of the stored data.</typeparam>
		/// <param name="callback">A callback to the </param>
		void OperateOnContents(OctreeCallback& callback)
		{
			if (nullptr != children) {
				for (int i = 0; i < 8; ++i)			// Octree can have up to 8 children
				{
					children[i].OperateOnContents(callback);
				}
			}
			else
			{
				if (!contents.empty())
				{
					callback(contents);
				}
			}
		}

		void Split()
		{
			using namespace DirectX;

			XMFLOAT3 halfSize;
			XMStoreFloat3(&halfSize, XMLoadFloat3(&size) / 2.0f);

			XMFLOAT3 position0; XMStoreFloat3(&position0, XMVectorAdd(XMLoadFloat3(&position), { -halfSize.x, -halfSize.y, -halfSize.z }));
			XMFLOAT3 position1; XMStoreFloat3(&position1, XMVectorAdd(XMLoadFloat3(&position), { halfSize.x, -halfSize.y, -halfSize.z }));
			XMFLOAT3 position2; XMStoreFloat3(&position2, XMVectorAdd(XMLoadFloat3(&position), { halfSize.x, -halfSize.y, halfSize.z }));
			XMFLOAT3 position3; XMStoreFloat3(&position3, XMVectorAdd(XMLoadFloat3(&position), { -halfSize.x, -halfSize.y, halfSize.z }));
			XMFLOAT3 position4; XMStoreFloat3(&position4, XMVectorAdd(XMLoadFloat3(&position), { -halfSize.x, halfSize.y, -halfSize.z }));
			XMFLOAT3 position5; XMStoreFloat3(&position5, XMVectorAdd(XMLoadFloat3(&position), { halfSize.x, halfSize.y, -halfSize.z }));
			XMFLOAT3 position6; XMStoreFloat3(&position6, XMVectorAdd(XMLoadFloat3(&position), { halfSize.x, halfSize.y, halfSize.z }));
			XMFLOAT3 position7; XMStoreFloat3(&position7, XMVectorAdd(XMLoadFloat3(&position), { -halfSize.x, halfSize.y, halfSize.z }));

			children = new OctreeNode<T>[8];

			children[0] = OctreeNode<T>(position0, halfSize);
			children[1] = OctreeNode<T>(position1, halfSize);
			children[2] = OctreeNode<T>(position2, halfSize);
			children[3] = OctreeNode<T>(position3, halfSize);
			children[4] = OctreeNode<T>(position4, halfSize);
			children[5] = OctreeNode<T>(position5, halfSize);
			children[6] = OctreeNode<T>(position6, halfSize);
			children[7] = OctreeNode<T>(position7, halfSize);
		}

		void Insert(const T& objectRef, const DirectX::XMFLOAT3& objectPos,
			const DirectX::XMFLOAT3& objectSize, int depthThreshold, const int maxSize)
		{
			// Check AABB. Does the other object belong in here?
			if (!CCE::Physics::CollideAABB(objectPos, objectSize, position, size))
			{
				return;
			}

			if (children)
			{
				// Doesn't belong here! Put in children
				for (int i = 0; i < 8; ++i) {
					children[i].Insert(objectRef, objectPos, objectSize, depthThreshold - 1, maxSize);
				}
			}
			else
			{
				contents.push_back(OctreeEntry<T>(&objectRef, objectPos, objectSize));

				// put contents in sub branches
				if (contents.size() > maxSize && depthThreshold > 0)
				{
					Split();

					for (const auto& content : contents)
					{
						for (int i = 0; i < 8; ++i)
						{
							children[i].Insert(*content.pObjRef, content.pos, content.size, depthThreshold - 1, maxSize);
						}
					}
					contents.clear(); // clear content!
				}
			}
		}

	protected:
		friend class Octree<T>;
		OctreeNode() { contents.reserve(5); }
		OctreeNode(const DirectX::XMFLOAT3 _position, const DirectX::XMFLOAT3& _size)
			: position(_position), size(_size), children(nullptr)
		{ contents.reserve(5); }
		~OctreeNode() { delete[] children; }
	
	protected:
		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT3 size{};

		OctreeNode<T>* children = nullptr;
		std::vector<OctreeEntry<T>> contents{};
	};

	template<typename T>
	class Octree
	{
	public:
		Octree(const DirectX::XMFLOAT3 _size, const int _depthThreshold = 10, const int _maxSize = 5)
			: root(new OctreeNode<T>({0, 0, 0}, _size)), depthThreshold(_depthThreshold), maxSize(_maxSize)
		{ }
		~Octree() { delete root; }

		void Insert(const T& object, const DirectX::XMFLOAT3& objPosition, const DirectX::XMFLOAT3& size)
		{
			root->Insert(object, objPosition, size, depthThreshold, maxSize);
		}

		void OperateOnContents(typename OctreeNode<T>::OctreeCallback func) {
			 root->OperateOnContents(func);
		}


	protected:
		OctreeNode<T>* root = nullptr;
		int depthThreshold = 10;
		int maxSize = 5;
	};
}

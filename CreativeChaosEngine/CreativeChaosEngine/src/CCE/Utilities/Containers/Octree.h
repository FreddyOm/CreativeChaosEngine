/*
* Inspired by this paper:
* https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/6accelerationstructures/Physics%20-%20Spatial%20Acceleration%20Structures.pdf
*/

#pragma once
#include "../../Graphics/Rendering/D3D11.h"
#include <functional>
#include <vector>

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

		T* pObjRef = nullptr;
		DirectX::XMFLOAT3 pos{};
		DirectX::XMFLOAT3 size{};
	};

	template<typename T>
	class OctreeNode
	{
	public:
		typedef std::function<void(std::vector<OctreeEntry<T>>&)> OctreeCallback;

		void OperateOnContents(OctreeCallback& callback);
		void Split();
		void Insert(const T& objectRef, const DirectX::XMFLOAT3& objectPos,
			const DirectX::XMFLOAT3& objectSize, int depthThreshold, const int maxSize);

	protected:
		friend class Octree<T>;
		OctreeNode() { contents.reserve(5); }
		OctreeNode(const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _size)
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

#include "Octree.h"
#include "../../Physics/Physics.h"

namespace CCE::Containers
{
	/// <summary>
	/// Calls the callback function on the children of the tree or, if none
	/// exist, in its content.
	/// </summary>
	/// <typeparam name="T">The data type of the stored data.</typeparam>
	/// <param name="callback">A callback to the </param>
	template<typename T>
	void OctreeNode<T>::OperateOnContents(std::function<void(std::vector<OctreeEntry<T>>&)>& callback)
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

	template<typename T>
	void OctreeNode<T>::Split()
	{
		using namespace DirectX;

		XMFLOAT3 halfSize = size / 2.0f;
		DASSERT(halfSize.x >= 0.1f,
			"Octree ran out of space. Minimum space is 0.1 units per axis.");

		children = new OctreeNode<T>[8];

		children[0] = OctreeNode<T>(position + XMFLOAT3(-halfSize.x, -halfSize.y, -halfSize.z), halfSize);
		children[1] = OctreeNode<T>(position + XMFLOAT3(halfSize.x, -halfSize.y, -halfSize.z), halfSize);
		children[2] = OctreeNode<T>(position + XMFLOAT3(halfSize.x, -halfSize.y, halfSize.z), halfSize);
		children[3] = OctreeNode<T>(position + XMFLOAT3(-halfSize.x, -halfSize.y, halfSize.z), halfSize);
		children[4] = OctreeNode<T>(position + XMFLOAT3(-halfSize.x, halfSize.y, -halfSize.z), halfSize);
		children[5] = OctreeNode<T>(position + XMFLOAT3(halfSize.x, halfSize.y, -halfSize.z), halfSize);
		children[6] = OctreeNode<T>(position + XMFLOAT3(halfSize.x, halfSize.y, halfSize.z), halfSize);
		children[7] = OctreeNode<T>(position + XMFLOAT3(-halfSize.x, halfSize.y, halfSize.z), halfSize);
	}

	template<typename T>
	void OctreeNode<T>::Insert(const T& objectRef, const DirectX::XMFLOAT3& objectPos,
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
			contents.push_back(OctreeEntry<T>(&objectRef,objectPos, objectSize));

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
}

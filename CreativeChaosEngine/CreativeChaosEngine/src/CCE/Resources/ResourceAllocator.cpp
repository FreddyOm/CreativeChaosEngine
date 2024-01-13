#include "ResourceAllocator.h"
#include "MeshLoader.h"

namespace CCE::Resources
{
	/// <summary>
	/// 
	/// </summary>
	/// <param name="meshPath"></param>
	/// <returns>A uniqe-pointer to the mesh data.</returns>
	std::shared_ptr<MeshData> ResourceAllocator::GetMesh(String meshPath)
	{
		// Since the mesh is usually only queried on model creation, we don't need to 
		// change the mesh data very often. We can use a hash map here in order to find
		// the data quickly.

		if (meshDataMap.find(meshPath.sId) == meshDataMap.end())
		{
			// Mesh not yet loaded! --> Load
			MeshLoader meshLoader;

			// Currently map the resource to its unique path! May change later!
			meshDataMap.at(meshPath.sId) = std::move(meshLoader.LoadResource(meshPath));
		}
		
		return meshDataMap.at(meshPath.sId);
	}

	ResourceAllocator* ResourceAllocator::Instance = nullptr;
}

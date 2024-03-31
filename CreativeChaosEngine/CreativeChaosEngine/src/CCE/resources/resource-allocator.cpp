#include "resource-allocator.h"
#include "mesh-loader.h"
#include "texture-loader.h"
#include "../../thirdparty/src/optick.h"

namespace CCE::Resources
{
	/// <summary>
	/// Tries to get the handle to a mesh with a given path and 
	/// loads the mesh if it doesn't exist already.
	/// </summary>
	/// <param name="meshPath">The path to a given mesh.</param>
	/// <returns>A shared-pointer to the mesh data.</returns>
	std::shared_ptr<MeshData> ResourceAllocator::GetMesh(String meshPath)
	{
		OPTICK_EVENT();
		// Since the mesh is usually only queried on model creation, we don't need to 
		// change the mesh data very often. We can use a hash map here in order to find
		// the data quickly.

		if (meshDataMap.find(meshPath.sId) == meshDataMap.end())
		{
			ScopedSpinLock lock(meshLoadSl);
			// Mesh not yet loaded! --> Load
			MeshLoader meshLoader;

			// Currently map the resource to its unique path! May change later!
			meshDataMap.insert({ meshPath.sId, std::move(meshLoader.LoadResource(meshPath)) });
		}
		
		return meshDataMap.at(meshPath.sId);
	}

	std::shared_ptr<TexData> ResourceAllocator::GetTexture(String texturePath)
	{
		OPTICK_EVENT();
		if (texDataMap.find(texturePath.sId) == texDataMap.end())
		{
			// Mesh not yet loaded! --> Load
			TextureLoader texLoader;

			// Currently map the resource to its unique path! May change later!
			texDataMap.insert({ texturePath.sId, std::move(texLoader.LoadResource(texturePath)) });
		}

		return texDataMap.at(texturePath.sId);
	}

	ResourceAllocator* ResourceAllocator::Instance = nullptr;
}

#pragma once
#include "ResourceLoader.h"
#include "../Graphics/Rendering/Vertex.h"
#include "../Core.h"

//#pragma comment(lib, "D:/Repositories/CreativeChaosEngine/CreativeChaosEngine/CreativeChaosEngine/resources/sdk/assimp-vc143-mt.lib")
#pragma comment(lib, "assimp-vc143-mt.lib")

#include "../include/assimp/Importer.hpp"

namespace CCE::Resources
{
	struct CCE_API MeshData
	{
		std::vector<CCE::Graphics::Vertex> VertexData = {};
		UINT64 VertexCount = 0;
	};

	struct CCE_API MeshLoader : ResourceLoader<MeshData>
	{
	public:
		MeshLoader();
		~MeshLoader();

		std::unique_ptr<MeshData> LoadResource(String filePath);
	private:
		Assimp::Importer* importer = nullptr;
	};
}

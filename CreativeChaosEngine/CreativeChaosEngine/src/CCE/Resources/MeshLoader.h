#pragma once
#include "ResourceLoader.h"
#include "../Graphics/Rendering/Vertex.h"
#include "../Core.h"
#pragma comment(lib, "assimp-vc143-mt.lib")
#include "../include/assimp/Importer.hpp"

namespace CCE::Resources
{
	struct CCE_API MeshData
	{
		MeshData()
		{
			VertexData = std::make_unique<std::vector<CCE::Graphics::Vertex>>();
			IndexBuffer = std::make_unique<std::vector<unsigned int>>();
		}

		~MeshData()
		{

		}

		UINT64 VertexCount = 0;
		std::shared_ptr<std::vector<CCE::Graphics::Vertex>> VertexData = {};
		std::shared_ptr<std::vector<unsigned int>> IndexBuffer;
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

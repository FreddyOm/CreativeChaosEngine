#pragma once
#pragma comment(lib, "assimp-vc143-mt.lib")
#include "../include/assimp/Importer.hpp"
#include "ResourceLoader.h"

namespace CCE::Resources
{
	struct MeshData;
	struct MeshLoader : ResourceLoader<MeshData>
	{
	public:
		MeshLoader();
		~MeshLoader();

		std::shared_ptr<MeshData> LoadResource(String filePath);
	private:
		Assimp::Importer* importer = nullptr;
	};
}

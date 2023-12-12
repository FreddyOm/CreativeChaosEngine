#include "MeshLoader.h"

namespace CCE::Resources
{
	MeshLoader::MeshLoader()
	{
		//importer = new Assimp::Importer();
	}

	MeshLoader::~MeshLoader()
	{
		//delete importer;
	}

	std::unique_ptr<MeshData> MeshLoader::LoadResource(String filePath)
	{
		// Load the data!

		
		return std::unique_ptr<MeshData>();
	}
}
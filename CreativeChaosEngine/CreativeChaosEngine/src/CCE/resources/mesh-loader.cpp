#include "mesh-loader.h"
#include "../analysis/debug.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh-data.h"
#include "../../thirdparty/src/optick.h"

namespace CCE::Resources
{
	MeshLoader::MeshLoader()
	{
		OPTICK_EVENT();
		importer = new Assimp::Importer();
	}

	MeshLoader::~MeshLoader()
	{
		OPTICK_EVENT();
		delete importer;
	}

	// @TODO: Load into central resource buffer and only load once of course!
	std::shared_ptr<MeshData> MeshLoader::LoadResource(String filePath)
	{
		OPTICK_EVENT();
		// Load the data!
		unsigned int flags = aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded |
			aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
		
		const aiScene* mesh = importer->ReadFile(filePath.Value(), flags);
		DASSERT(nullptr != mesh, "Failed loading the mesh from file!");

		MeshData data;
		data.VertexCount = mesh->mMeshes[0]->mNumVertices;
		data.VertexData->reserve(data.VertexCount);

		for (size_t i = 0; i < 1; ++i)  /*i < mesh->mNumMeshes*/
		{
			// Vertex data
			for (size_t v = 0; v < mesh->mMeshes[i]->mNumVertices; ++v)
			{
				// Vertices
				DirectX::XMFLOAT3 vertex = {};
				DirectX::XMStoreFloat3(&vertex, 
					{ mesh->mMeshes[i]->mVertices[v].x, mesh->mMeshes[i]->mVertices[v].y, mesh->mMeshes[i]->mVertices[v].z });

				// UVs
				DirectX::XMFLOAT2 uv = {};
				unsigned int channels = mesh->mMeshes[i]->GetNumUVChannels();
				if (channels > 0 && mesh->mMeshes[i]->mTextureCoords[0] != nullptr)
				{
					DirectX::XMStoreFloat2(&uv, { mesh->mMeshes[i]->mTextureCoords[0][v].x, mesh->mMeshes[i]->mTextureCoords[0][v].y });
				}

				data.VertexData->push_back({ std::move(vertex), {std::move(uv)}});
			}

			// Index Buffer
			for (size_t f = 0; f < mesh->mMeshes[i]->mNumFaces; ++f)
			{
				for(size_t fi = 0; fi < mesh->mMeshes[i]->mFaces[f].mNumIndices; ++fi)
					data.IndexBuffer->push_back(mesh->mMeshes[i]->mFaces[f].mIndices[fi]);
			}
		}
		
		return std::make_shared<MeshData>(std::move(data));
	}
}
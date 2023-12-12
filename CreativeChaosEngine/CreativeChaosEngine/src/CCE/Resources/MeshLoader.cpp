#include "MeshLoader.h"
#include "../Analysis/Debug.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace CCE::Resources
{
	MeshLoader::MeshLoader()
	{
		importer = new Assimp::Importer();
	}

	MeshLoader::~MeshLoader()
	{
		delete importer;
	}

	// TODO: Load into central resource buffer and only load once of course!
	std::unique_ptr<MeshData> MeshLoader::LoadResource(String filePath)
	{
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

				data.VertexData->push_back({ std::move(vertex), {} });
			}
			/*
			DirectX::XMFLOAT2 uv = {};
			if (mesh->mMeshes[i]->mTextureCoords[v] != nullptr)
			{
				DirectX::XMStoreFloat2(&uv, { mesh->mMeshes[i]->mTextureCoords[v]->x , mesh->mMeshes[i]->mTextureCoords[v]->y });
			}
			*/			

			// Index Buffer
			for (size_t f = 0; f < mesh->mMeshes[i]->mNumFaces; ++f)
			{
				for(size_t fi = 0; fi < mesh->mMeshes[i]->mFaces[f].mNumIndices; ++fi)
					data.IndexBuffer->push_back(mesh->mMeshes[i]->mFaces[f].mIndices[fi]);
			}
		}
		
		return std::make_unique<MeshData>(std::move(data));
	}
}
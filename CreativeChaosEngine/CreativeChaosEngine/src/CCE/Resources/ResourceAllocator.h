#pragma once
#include <memory>
#include <unordered_map>
#include "../String/String.h"
#include "ResourceLoader.h"

namespace CCE::Resources
{
	struct MeshData;
	struct TexData;
	struct ResourceAllocator
	{
	public:
		static ResourceAllocator* Instance;
	public:
		ResourceAllocator()
		{
			Instance = this;
		}

		~ResourceAllocator() = default;

		std::shared_ptr<MeshData> GetMesh(String meshPath);
		std::shared_ptr<TexData> GetTexture(String texturePath);

	
		std::unordered_map<UINT64, std::shared_ptr<MeshData>> meshDataMap = {};
		std::unordered_map<UINT64, std::shared_ptr<TexData>> texDataMap = {};
	};
}
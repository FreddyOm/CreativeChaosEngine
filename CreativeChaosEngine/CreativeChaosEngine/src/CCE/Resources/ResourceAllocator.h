#pragma once
#include <memory>
#include <unordered_map>
#include "../String/String.h"

namespace CCE::Resources
{
	struct MeshData;
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

	private:
		std::unordered_map<UINT64, std::shared_ptr<MeshData>> meshDataMap = {};
	};
}
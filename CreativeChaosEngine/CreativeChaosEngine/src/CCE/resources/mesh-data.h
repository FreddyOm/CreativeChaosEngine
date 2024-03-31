#pragma once
#include "../Graphics/Rendering/Vertex.h"
#include <memory>

namespace CCE::Resources
{
	struct MeshData
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
		std::shared_ptr<std::vector<unsigned int>> IndexBuffer = {};
	};
}

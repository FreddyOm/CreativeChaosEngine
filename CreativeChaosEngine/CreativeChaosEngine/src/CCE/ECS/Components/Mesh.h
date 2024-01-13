#pragma once
#include <memory>
#include "../../String/String.h"
#include "../../Resources/MeshData.h"

namespace CCE::ECS::Components
{
	class Mesh
	{
	public:
		Mesh() {};
		Mesh(String path);
		~Mesh();

		String Path() const;
		std::shared_ptr<Resources::MeshData> Data() const;

		// Operational functions...

	private:
		String meshPath = "";
		std::shared_ptr<Resources::MeshData> meshData{};
	};
}

#include "Mesh.h"
#include "../../Resources/ResourceAllocator.h"
#include "../../Resources/MeshLoader.h"

namespace CCE::ECS::Components
{
	Mesh::Mesh(String path)
		: meshPath(path)
		,meshData(CCE::Resources::ResourceAllocator::Instance->GetMesh(path))
	{
		// @TODO: check for failure of resource loading. What if the path is invalid?
	}

	Mesh::~Mesh()
	{
		meshPath = "";
	}

	String Mesh::Path() const
	{
		return meshPath;
	}

	std::shared_ptr<Resources::MeshData> Mesh::Data() const
	{
		return meshData;
	}
}

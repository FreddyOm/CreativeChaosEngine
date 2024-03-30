#include "Mesh.h"
#include "../../resources/mesh-loader.h"
#include "../../resources/resource-allocator.h"
#include "../../graphics/rendering/bindable/bindable-common.h"
#include "../../string/string-converter.h"
#include "../../manager/application.h"
#include "../../analysis/debug.h"

namespace CCE::ECS::Components
{
	Mesh::Mesh(String path)
		: meshPath(path)
		,meshData(Resources::ResourceAllocator::Instance->GetMesh(path))
	{
		OPTICK_EVENT();
		String vertexShader = Application::Instance->resourceDataPath.Path() + "/shader/DefaultVertexShader.cso";
		auto vs = std::make_shared<Graphics::VertexShader>(StringConverter::StringToWString(vertexShader.Value()));

		AddBind(std::move(std::make_shared<Graphics::IndexBuffer>(meshData->IndexBuffer)));
		AddBind(std::move(std::make_shared<Graphics::VertexBuffer>(meshData->VertexData)));
		AddBind(std::move(std::make_shared<Graphics::InputLayout>(vs->GetBytecode())));
		AddBind(std::move(vs));
		AddBind(std::move(std::make_shared<Graphics::Topology>()));
	}

	Mesh::~Mesh()
	{
		meshPath = "";
	}

	String Mesh::Path() const
	{
		OPTICK_EVENT();
		return meshPath;
	}

	std::shared_ptr<Resources::MeshData> Mesh::Data() const
	{
		OPTICK_EVENT();
		return meshData;
	}

	void Mesh::StaticBind()
	{
		OPTICK_EVENT();
	}

	void Mesh::DynamicBind(DirectX::XMMATRIX modelMatrix)
	{
		OPTICK_EVENT();
		for (auto& bind : meshBindPtr)
		{
			bind->DynamicBind();
		}
	}

	void Mesh::AddBind(std::shared_ptr<Graphics::IBindable> bind) noexcept
	{
		OPTICK_EVENT();
		if (typeid(*bind) == typeid(Graphics::IndexBuffer))
		{
			DASSERT(pIndexBuffer == nullptr, "Binding multiple index buffers is not allowed!");
			pIndexBuffer = &static_cast<Graphics::IndexBuffer&>(*bind);
		}
		meshBindPtr.push_back(std::move(bind));
	}

	void Mesh::CreateConstBufs(const Graphics::VSConstBufData& constBufData)
	{
		OPTICK_EVENT();
		pMeshConstBuf = std::make_shared<Graphics::VSConstantBuffer<Graphics::VSConstBufData>>(constBufData, 0);
	}
}

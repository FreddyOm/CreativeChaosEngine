#include "Mesh.h"
#include "../../Resources/MeshLoader.h"
#include "../../Resources/ResourceAllocator.h"
#include "../../Graphics/Rendering/Bindable/BindableCommon.h"
#include "../../String/StringConverter.h"
#include "../../Manager/Application.h"
#include "../../Analysis/Debug.h"

namespace CCE::ECS::Components
{
	Mesh::Mesh(String path)
		: meshPath(path)
		,meshData(Resources::ResourceAllocator::Instance->GetMesh(path))
	{
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
		return meshPath;
	}

	std::shared_ptr<Resources::MeshData> Mesh::Data() const
	{
		return meshData;
	}

	void Mesh::StaticBind()
	{

	}

	void Mesh::DynamicBind(DirectX::XMMATRIX modelMatrix)
	{
		for (auto& bind : meshBindPtr)
		{
			bind->DynamicBind();
		}
	}

	void Mesh::AddBind(std::shared_ptr<Graphics::IBindable> bind) noexcept
	{
		if (typeid(*bind) == typeid(Graphics::IndexBuffer))
		{
			DASSERT(pIndexBuffer == nullptr, "Binding multiple index buffers is not allowed!");
			pIndexBuffer = &static_cast<Graphics::IndexBuffer&>(*bind);
		}
		meshBindPtr.push_back(std::move(bind));
	}

	void Mesh::CreateConstBufs(const Graphics::VSConstBufData& constBufData)
	{
		pMeshConstBuf = std::make_shared<Graphics::VSConstantBuffer<Graphics::VSConstBufData>>(constBufData, 0);
	}
}

#include "Mesh.h"

namespace CCE::Graphics
{
	Mesh::Mesh()
	{

		std::vector<Vertex> vertices =
		{
			{ XMFLOAT3(0.0f, 0.3f, 0.0f) },
			{ XMFLOAT3(0.2f, -0.2f, 0.0f) },
			{ XMFLOAT3(-0.2f, -0.2f, 0.0f) },
		};

		std::vector<unsigned int> indices = 
		{
			0,1,2
		};

		//ps = PixelShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultPixelShader.cso");
		//vs = VertexShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultVertexShader.cso");
	}

	Mesh::~Mesh()
	{
		for (auto bind : binds)
		{
			bind.reset();
		}

		binds.clear();
	}

	void Mesh::DrawIndexed(UINT count)
	{
		RenderPipeline::Instance->GetDeviceContextPtr()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		for (auto bind : binds)
		{
			bind->Bind();
		}
		RenderPipeline::Instance->GetDeviceContextPtr()->Draw(count, 0u);
	}

	void Mesh::AddBind(std::shared_ptr<IBindable> bindable) noexcept
	{
		binds.push_back(bindable);
	}
}

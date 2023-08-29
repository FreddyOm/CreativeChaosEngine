#include "Mesh.h"

namespace CCE::Graphics
{
	Mesh::Mesh()
		: ps(PixelShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultPixelShader.cso")),
		vs(VertexShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultVertexShader.cso"))
	{
		ID3D11InputLayout* pInputLayout;

		// Layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HRESULT hr = RenderPipeline::Instance->GetDevicePtr()->CreateInputLayout(layout, 1, vs.GetBytecode()->GetBufferPointer(),
			vs.GetBytecode()->GetBufferSize(), &pInputLayout);
		DASSERT(hr == S_OK, "Failed creating input layout resource.");

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

		indexBuf = new IndexBuffer(indices);
		vertexBuf = new VertexBuffer(vertices);

		//ps = PixelShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultPixelShader.cso");
		//vs = VertexShader(L"D:/Repos/CreativeChaosEngine/CreativeChaosEngine/bin/Debug-x64/CreativeChaosEditor/resources/shader/DefaultVertexShader.cso");
	}

	Mesh::~Mesh()
	{
		vertexBuf->~VertexBuffer();
		indexBuf->~IndexBuffer();
		delete vertexBuf;
		delete indexBuf;
	}

	void Mesh::DrawIndexed(UINT count)
	{
		RenderPipeline::Instance->GetDeviceContextPtr()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		vertexBuf->Bind();
		//indexBuf->Bind();
		vs.Bind();
		ps.Bind();
		RenderPipeline::Instance->GetDeviceContextPtr()->Draw(count, 0u);
	}
}

#include "VertexBuffer.h"

namespace CCE::Graphics
{
	VertexBuffer::VertexBuffer(std::vector<Vertex>& vertexBuffer)
		: count(vertexBuffer.size())
	{
		// Create buffer description

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = UINT64(sizeof(Vertex) * 3);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertexBuffer.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		DASSERT(GetDevice(RenderPipeline::Instance)->CreateBuffer(
		&bufferDesc, &initData, &pVertexBuffer) == S_OK, "Failed creating the vertex buffer!");
	}

	VertexBuffer::~VertexBuffer()
	{
		pVertexBuffer.Reset();
	}

	void VertexBuffer::Bind()
	{
		const UINT offset = 0u;
		const UINT stride = sizeof(XMFLOAT3);
		GetContext(RenderPipeline::Instance)->IASetVertexBuffers(
			0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}
}

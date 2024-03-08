#include "VertexBuffer.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
	VertexBuffer::VertexBuffer(std::shared_ptr<std::vector<Vertex>> vertexBuffer)
		: count(vertexBuffer->size())
	{
		OPTICK_EVENT();
		// Create buffer description

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = UINT64(sizeof(Vertex) * count);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertexBuffer.get()->data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		DASSERT(GetDevice()->CreateBuffer(
		&bufferDesc, &initData, &pVertexBuffer) == S_OK, "Failed creating the vertex buffer!");
	}

	VertexBuffer::~VertexBuffer()
	{
		OPTICK_EVENT();
		pVertexBuffer.Reset();
	}

	void VertexBuffer::DynamicBind()
	{
		OPTICK_EVENT();
		const UINT offset = 0u;
		const UINT stride = sizeof(Vertex);
		GetContext()->IASetVertexBuffers(
			0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	}

	void VertexBuffer::StaticBind()
	{
		OPTICK_EVENT();
	}
}

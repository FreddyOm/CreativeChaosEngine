#include "index-buffer.h"
#include "../../../analysis/debug.h"

namespace CCE::Graphics
{
	IndexBuffer::IndexBuffer(std::shared_ptr<std::vector<unsigned int>> indices)
		: count(indices->size())
	{
		OPTICK_EVENT();
		// Create buffer description
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT64(count * sizeof(unsigned int));
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.get()->data();
		
		DASSERT(g_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer) == S_OK,
			"Failed creating the index buffer!");
	}

	IndexBuffer::~IndexBuffer()
	{
		OPTICK_EVENT();
		pIndexBuffer.Reset();
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		OPTICK_EVENT();
		return count;
	}

	void IndexBuffer::DynamicBind()
	{
		OPTICK_EVENT();
		// Bind to pipeline
		g_pContext->IASetIndexBuffer(
			pIndexBuffer.Get(), 
			DXGI_FORMAT_R32_UINT, 
			0u);
	}

	void IndexBuffer::StaticBind()
	{
		OPTICK_EVENT();
	}
}

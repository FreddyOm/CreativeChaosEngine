#include "IndexBuffer.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
	IndexBuffer::IndexBuffer(std::vector<DWORD>& indices)
		: count(indices.size())
	{
		// Create buffer description
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT64(count * sizeof(DWORD));
		ibd.StructureByteStride = sizeof(DWORD);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		
		DASSERT(GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer) == S_OK,
			"Failed creating the index buffer!");
	}

	IndexBuffer::~IndexBuffer()
	{
		pIndexBuffer.Reset();
	}

	UINT IndexBuffer::GetCount() const noexcept
	{
		return count;
	}

	void IndexBuffer::Bind()
	{
		// Bind to pipeline
		GetContext()->IASetIndexBuffer(
			pIndexBuffer.Get(), 
			DXGI_FORMAT_R32_UINT, 
			0u);
	}
}

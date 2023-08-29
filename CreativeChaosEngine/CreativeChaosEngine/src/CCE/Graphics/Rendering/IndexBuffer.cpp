#include "IndexBuffer.h"
#include "../../Analysis/Debug.h"

namespace CCE::Graphics
{
	IndexBuffer::IndexBuffer(std::vector<unsigned int>& indices) 
		: count(indices.size())
	{
		// Create buffer description
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(count * sizeof(unsigned int));
		ibd.StructureByteStride = sizeof(unsigned int);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		
		DASSERT(RenderPipeline::Instance->GetDevicePtr()->CreateBuffer(&ibd, &isd, &pIndexBuffer) == S_OK, 
			"Failed creating the index buffer!");
	}

	IndexBuffer::~IndexBuffer()
	{
		pIndexBuffer.Reset();
	}

	void IndexBuffer::Bind()
	{
		// Bind to pipeline
		RenderPipeline::Instance->GetDeviceContextPtr()->IASetIndexBuffer(
			pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}
}

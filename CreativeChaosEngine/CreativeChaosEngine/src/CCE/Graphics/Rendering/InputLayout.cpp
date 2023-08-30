#include "InputLayout.h"

namespace CCE::Graphics
{
	InputLayout::InputLayout(ID3DBlob* pVertexShaderBytecode)
	{
		// Create Input Layout

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HRESULT hr = RenderPipeline::Instance->GetDevicePtr()->CreateInputLayout(
			layout, 1, pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(), &pInputLayout);
		DASSERT(hr == S_OK, "Failed creating input layout resource.");
	}

	InputLayout::~InputLayout()
	{
		pInputLayout.Reset();
	}

	void InputLayout::Bind()
	{
		RenderPipeline::Instance->GetDeviceContextPtr()->IASetInputLayout(pInputLayout.Get());
	}
}

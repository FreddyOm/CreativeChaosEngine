#include "input-layout.h"
#include "../../../analysis/debug.h"
#include "../../rendering/D3D11/d3d11RenderPipeline.h"

namespace CCE::Graphics
{
	InputLayout::InputLayout(ID3DBlob* pVertexShaderBytecode)
	{
		OPTICK_EVENT();
		// Create Input Layout for vertex shader

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		int iNumElements = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
		HRESULT hr = g_pDevice->CreateInputLayout(
			layout, iNumElements, pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(), &pInputLayout);
		DASSERT(hr == S_OK, "Failed creating input layout resource.");
	}

	InputLayout::~InputLayout()
	{
		OPTICK_EVENT();
		pInputLayout.Reset();
	}

	void InputLayout::DynamicBind()
	{
		OPTICK_EVENT();
		g_pContext->IASetInputLayout(pInputLayout.Get());
	}

	void InputLayout::StaticBind()
	{
		OPTICK_EVENT();
	}
}

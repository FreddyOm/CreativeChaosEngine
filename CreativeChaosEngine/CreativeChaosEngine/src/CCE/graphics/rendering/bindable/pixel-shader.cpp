#include "pixel-shader.h"
#include "../../../analysis/debug.h"

namespace CCE::Graphics
{
	PixelShader::PixelShader(const std::wstring resourcePath)
	{
		OPTICK_EVENT();
		// Create PS resource from file
		ComPtr<ID3DBlob> pBlob;
		HRESULT hr = D3DReadFileToBlob(resourcePath.c_str(), &pBlob);
		DASSERT(hr == S_OK, "Failed reading the pixel shader.");
		// Check shader version for older cpus / gpus (my laptop!! :O)
		hr = g_pDevice->CreatePixelShader(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
		DASSERT( hr == S_OK, "Failed creating the pixel shader resource.");
	}

	PixelShader::~PixelShader()
	{
		OPTICK_EVENT();
		pPixelShader.Reset();
	}

	void PixelShader::DynamicBind()
	{
		OPTICK_EVENT();
		g_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	void PixelShader::StaticBind()
	{
		OPTICK_EVENT();
	}
}

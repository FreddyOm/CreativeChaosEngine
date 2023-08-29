#include "PixelShader.h"

namespace CCE::Graphics
{
	PixelShader::PixelShader(const std::wstring resourcePath)
	{
		// Create PS resource from file
		ComPtr<ID3DBlob> pBlob;
		HRESULT hr = D3DReadFileToBlob(resourcePath.c_str(), &pBlob);
		DASSERT(hr == S_OK,
			"Failed reading the pixel shader.");
		DASSERT(RenderPipeline::Instance->GetDevicePtr()->CreatePixelShader(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader) == S_OK,
			"Failed creating the pixel shader resource.");
	}

	PixelShader::~PixelShader()
	{
		pPixelShader.Reset();
	}

	void PixelShader::Bind()
	{
		RenderPipeline::Instance->GetDeviceContextPtr()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
}

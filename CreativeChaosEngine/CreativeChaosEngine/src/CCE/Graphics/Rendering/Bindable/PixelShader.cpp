#include "PixelShader.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
	PixelShader::PixelShader(const std::wstring resourcePath)
	{
		// Create PS resource from file
		ComPtr<ID3DBlob> pBlob;
		HRESULT hr = D3DReadFileToBlob(resourcePath.c_str(), &pBlob);
		DASSERT(hr == S_OK, "Failed reading the pixel shader.");
		// Check shader version for older cpus / gpus (my laptop!! :O)
		hr = GetDevice()->CreatePixelShader(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
		DASSERT( hr == S_OK, "Failed creating the pixel shader resource.");
	}

	PixelShader::~PixelShader()
	{
		pPixelShader.Reset();
	}

	void PixelShader::DynamicBind()
	{
		GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}

	void PixelShader::StaticBind()
	{

	}
}

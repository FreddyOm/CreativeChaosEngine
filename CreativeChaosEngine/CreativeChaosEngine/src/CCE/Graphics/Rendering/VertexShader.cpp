#include "VertexShader.h"

namespace CCE::Graphics
{
	VertexShader::VertexShader(const std::wstring resourcePath)
	{
		// Create VS resource from file
		HRESULT hr;
		hr = D3DReadFileToBlob(resourcePath.c_str(), &pBytecodeBlob);
		DASSERT(hr == S_OK, "Failed reading vertex shader.");

		hr = RenderPipeline::Instance->GetDevicePtr()->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader);
		DASSERT(hr == S_OK, "Failed creating the vertex shader resource.");
	}

	VertexShader::~VertexShader()
	{
		pBytecodeBlob.Reset();
		pVertexShader.Reset();
	}

	void VertexShader::Bind()
	{
		RenderPipeline::Instance->GetDeviceContextPtr()->VSSetShader(
			pVertexShader.Get(), nullptr, 0u);
	}
	ID3DBlob* VertexShader::GetBytecode() const
	{
		return pBytecodeBlob.Get();
	}

	ID3D11VertexShader* VertexShader::GetVertexShader() const
	{
		return pVertexShader.Get();
	}
}

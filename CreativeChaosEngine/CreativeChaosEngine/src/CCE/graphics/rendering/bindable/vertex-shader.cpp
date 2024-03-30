#include "vertex-shader.h"
#include "../../../analysis/debug.h"

namespace CCE::Graphics
{
	VertexShader::VertexShader(const std::wstring resourcePath)
	{
		OPTICK_EVENT();
		// Create VS resource from file
		HRESULT hr;
		hr = D3DReadFileToBlob(resourcePath.c_str(), &pBytecodeBlob);
		DASSERT(hr == S_OK, "Failed reading vertex shader file.");

		hr = g_pDevice->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
			pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader);
		DASSERT(hr == S_OK, "Failed creating the vertex shader resource.");
	}

	VertexShader::~VertexShader()
	{
		OPTICK_EVENT();
		pBytecodeBlob.Reset();
		pVertexShader.Reset();
	}

	void VertexShader::DynamicBind()
	{
		OPTICK_EVENT();
		g_pContext->VSSetShader(
			pVertexShader.Get(), nullptr, 0u);
	}

	void VertexShader::StaticBind()
	{
		OPTICK_EVENT();
	}

	ID3DBlob* VertexShader::GetBytecode() const
	{
		OPTICK_EVENT();
		return pBytecodeBlob.Get();
	}

	ID3D11VertexShader* VertexShader::GetVertexShader() const
	{
		OPTICK_EVENT();
		return pVertexShader.Get();
	}
}

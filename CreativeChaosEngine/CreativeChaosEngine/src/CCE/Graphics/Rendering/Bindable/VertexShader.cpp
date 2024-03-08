#include "VertexShader.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
	VertexShader::VertexShader(const std::wstring resourcePath)
	{
		OPTICK_EVENT();
		// Create VS resource from file
		HRESULT hr;
		hr = D3DReadFileToBlob(resourcePath.c_str(), &pBytecodeBlob);
		DASSERT(hr == S_OK, "Failed reading vertex shader file.");

		hr = GetDevice()->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
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
		GetContext()->VSSetShader(
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

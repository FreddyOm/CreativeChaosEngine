#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include "D3D11.h"
#include <string>

namespace CCE::Graphics
{
	struct CCE_API VertexShader : public IBindable
	{
	public:
		VertexShader(const std::wstring resourcePath);
		~VertexShader();

		// Geerbt über IBindable
		void Bind() override;
		ID3DBlob* GetBytecode() const;
		ID3D11VertexShader* GetVertexShader() const;

	private:
		ComPtr<ID3DBlob> pBytecodeBlob;
		ComPtr<ID3D11VertexShader> pVertexShader;
	};
}

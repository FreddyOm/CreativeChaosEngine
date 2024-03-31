#pragma once
#include "IBindable.h"
#include "../../../Core.h"
#include <string>

namespace CCE::Graphics
{
	struct VertexShader : public IBindable
	{
	public:
		VertexShader(const std::wstring resourcePath);
		~VertexShader();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

		ID3DBlob* GetBytecode() const;
		ID3D11VertexShader* GetVertexShader() const;

	private:
		ComPtr<ID3DBlob> pBytecodeBlob;
		ComPtr<ID3D11VertexShader> pVertexShader;
	};
}

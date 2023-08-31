#pragma once
#include "IBindable.h"
#include "../../../Core.h"

namespace CCE::Graphics
{
	struct CCE_API InputLayout : public IBindable
	{
	public:
		InputLayout(ID3DBlob* pVertexShaderBytecode);
		~InputLayout();

		// Geerbt über IBindable
		virtual void Bind() override;

	private:
		ComPtr<ID3D11InputLayout> pInputLayout;
	};
}

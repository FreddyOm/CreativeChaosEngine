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
		void DynamicBind() override;
		void StaticBind() override;

	private:
		ComPtr<ID3D11InputLayout> pInputLayout;
	};
}

#pragma once
#include "IBindable.h"
#include "../../Core.h"
#include <string>

namespace CCE::Graphics
{
	struct CCE_API PixelShader : public IBindable
	{
	public:
		PixelShader(const std::wstring resourcePath);
		~PixelShader();

		ID3D11PixelShader* GetPixelShader() const
		{
			return pPixelShader.Get();
		}

		// Geerbt über IBindable
		void Bind() override;

	private:
		ComPtr<ID3D11PixelShader> pPixelShader;
	};
}

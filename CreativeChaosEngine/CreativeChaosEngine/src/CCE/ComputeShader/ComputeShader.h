#pragma once
#include <string>
#include "../Graphics/Rendering/D3D11.h"

namespace CCE::Acceleration
{
	struct ComputeShader
	{
	public:
		ComputeShader(const std::wstring resourcePath);
		~ComputeShader();

		ID3D11ComputeShader* GetPixelShader() const
		{
			return pComputeShader.Get();
		}

		void DynamicBind();
		void StaticBind();

	private:
		ComPtr<ID3D11ComputeShader> pComputeShader;
	};
}
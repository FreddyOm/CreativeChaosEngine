#pragma once
#include "IBindable.h"

namespace CCE::Graphics
{
	struct CCE_API Sampler : public IBindable
	{
	public:
		Sampler(D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP);
		~Sampler();

		// Geerbt über IBindable
		void DynamicBind() override;
		void StaticBind() override;

	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler = {};
	};
}
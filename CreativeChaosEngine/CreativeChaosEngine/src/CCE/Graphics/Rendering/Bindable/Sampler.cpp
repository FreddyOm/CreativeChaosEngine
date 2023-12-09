#include "Sampler.h"
#include "../../../Analysis/Debug.h"

namespace CCE::Graphics
{
	Sampler::Sampler(D3D11_TEXTURE_ADDRESS_MODE addressMode)
	{
		D3D11_SAMPLER_DESC smplr = {};
		smplr.AddressU = addressMode;
		smplr.AddressW = addressMode;
		smplr.AddressV = addressMode;
		smplr.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		DASSERT(GetDevice()->CreateSamplerState(&smplr, &pSampler) == S_OK,
			"Failed creating the sampler state!");
		
	}

	Sampler::~Sampler()
	{
		pSampler.Reset();
	}

	void Sampler::DynamicBind()
	{
		GetContext()->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}

	void Sampler::StaticBind()
	{
		
	}
}
#include "ComputeShader.h"
#include "../Graphics/RenderPipeline.h"
#include "../Analysis/Debug.h"

namespace CCE::Acceleration
{
	ComputeShader::ComputeShader(const std::wstring resourcePath)
	{
		using RP = Graphics::RenderPipeline;

		if (RP::Instance->GetDevicePtr()->GetFeatureLevel() < D3D_FEATURE_LEVEL_11_0)
		{
			D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwopts = { 0 };
			(void)RP::Instance->GetDevicePtr()->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwopts, sizeof(hwopts));
			if (!hwopts.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
			{
				RP::Instance->GetDevicePtr()->Release();
				DERROR("DirectCompute is not supported by this device\n");
				return;
			}
		}

		ComPtr<ID3DBlob> pBlob;
		HRESULT hr = D3DReadFileToBlob(resourcePath.c_str(), &pBlob);
		DASSERT(hr == S_OK, "Failed reading the pixel shader.");
		// Check shader version for older cpus / gpus (my laptop!! :O)
		hr = RP::Instance->GetDevicePtr()->CreateComputeShader(
			pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pComputeShader);
		DASSERT(hr == S_OK, "Failed creating the compute shader resource.");
	}

	ComputeShader::~ComputeShader()
	{
		pComputeShader.Reset();
	}

	void ComputeShader::DynamicBind()
	{

	}

	void ComputeShader::StaticBind()
	{
		using RP = Graphics::RenderPipeline;

		RP::Instance->GetDeviceContextPtr()->CSSetShader(pComputeShader.Get(), nullptr, 0u);
	}
}

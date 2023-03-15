#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "../Memory/StackAllocator.h"
#include "../Utilities/Color/Color.h"

namespace CCE::Graphics
{
	using Microsoft::WRL::ComPtr;
	using CCE::Color;

	class CCE_API RenderPipeline
	{
	public:
		RenderPipeline() = default;
		~RenderPipeline()
		{
			pDevice.~ComPtr();
			pSwapChain.~ComPtr();
			pContext.~ComPtr();
			pRenderTarget.~ComPtr();
			pDSV.~ComPtr();
			pBackBuffer.~ComPtr();
		}

	public:
		void InitializeD3D11(const HWND hWnd, const int width, const int height);
		void BeginFrame(const Color col) const;
		void EndFrame();

	private:
		ComPtr<ID3D11Device> pDevice = nullptr;
		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		ComPtr<ID3D11DeviceContext> pContext = nullptr;
		ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
		ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
		ComPtr<ID3D11Resource> pBackBuffer = nullptr;

		// TODO: Load from config
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		D3D_FEATURE_LEVEL featureLvl[3] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
		const DXGI_MODE_DESC dxgiModeDesc
		{
			0,										// width
			0,										// height
			DXGI_RATIONAL{0,0},						// refresh_rate
			DXGI_FORMAT_B8G8R8A8_UNORM,				// color format
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,	// scanline order
			DXGI_MODE_SCALING_UNSPECIFIED			// scaling mode
		};
		DXGI_SAMPLE_DESC dxgiSampleDesc
		{
			1,	// multisamples per pixel
			0	// image quality lvl
		};
	};
}

#pragma once
#include "BaseManager.h"
#include <d3d11.h>
#include <wrl.h>
#include "../Memory/StackAllocator.h"

namespace CCE
{
	struct CCE_API RenderManager : public BaseManager
	{
	public:
		RenderManager() = default;
		~RenderManager() = default;

		void StartUp() override;
		void ShutDown() override;

		static RenderManager* Instance;
	
	public:
		void InitializeD3D11(const HWND hWnd);

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer = nullptr;

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

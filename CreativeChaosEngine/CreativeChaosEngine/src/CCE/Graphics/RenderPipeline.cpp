#include "RenderPipeline.h"
#include "../Analysis/Logger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace CCE::Graphics
{
	/// <summary>
	/// Initializes the D3D11 
	/// </summary>
	/// <param name="hWnd"></param>
	void RenderPipeline::InitializeD3D11(const HWND hWnd, const int width, const int height)
	{
		LOG_REND("Initializing Direct3D...");

		// TODO: Load from config
		// swap chain
		const DXGI_SWAP_CHAIN_DESC _swapChainDesc =
		{
			dxgiModeDesc,						// backbuffer settings
			dxgiSampleDesc,						// sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,	// swap chain is used as back buffer
			2,									// amount of swap chain buffers
			hWnd,								// handle to window
			TRUE,								// windowed or fullscreen (?)
			DXGI_SWAP_EFFECT_FLIP_DISCARD,		// swap fx
			NULL								// flags
		};

		swapChainDesc = _swapChainDesc;

		LOG_REND("Creating Swapchain & Device...");
		// create device and swapchain
		HRESULT cdasc = D3D11CreateDeviceAndSwapChain(
			NULL,									// default adapter
			D3D_DRIVER_TYPE_HARDWARE,				// driver type
			NULL,									// software module (Default: NULL)
			D3D10_CREATE_DEVICE_SINGLETHREADED |
#ifdef DEBUG
			D3D11_CREATE_DEVICE_DEBUG |				// for debug use
			D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS |
#elif PROFILING
			D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS |
#elif NDEBUG
			D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY |
#endif
			D3D11_CREATE_DEVICE_BGRA_SUPPORT,		// for interuse of D3D and D2D
			nullptr,								// featureLvl
			0,										// length of feature Lvl
			D3D11_SDK_VERSION,						// SDK-Version
			&swapChainDesc,							// swap chain description pointer
			&pSwapChain,							// swap chain pointer
			&pDevice,								// device pointer
			&featureLvl[0],							// feautre lvl pointer
			&pContext								// context pointer
		);

		DERROR((HRESULT)cdasc);
		DASSERT(cdasc == S_OK, "Creating Device and Swapchain unsuccessful!");

		HRESULT sc = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
		DERROR((HRESULT)sc);
		DASSERT(sc == S_OK, "Getting buffer was unsuccessful!");

		LOG_REND("Creating Render Target View...");

		HRESULT crtv = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTarget);
		DERROR((HRESULT)crtv);
		DASSERT(crtv == S_OK, "Creating render target view was unsuccessful!");

		// check again
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		LOG_REND("Creating Depth Stencil...");

		// creating depth stencil state
		ComPtr<ID3D11DepthStencilState> pDSState;
		DASSERT(pDevice->CreateDepthStencilState(&dsDesc, &pDSState) == S_OK,
			"Failed creating a depth stencil state!");

		// bind depth state
		pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// create depth stencil texture
		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil) == S_OK,
			"Failed creating depth stencil texture!");

		// create view of depth stencil tex
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;

		DASSERT(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());

		// configure viewport
		D3D11_VIEWPORT vp;
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pContext->RSSetViewports(1u, &vp);
	}

	void RenderPipeline::BeginFrame(const Color col) const
	{
		const float color[] = { col.r, col.g, col.b, col.a};
		// Clear render view and draw background color
		pContext->ClearRenderTargetView(pRenderTarget.Get(), color);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

		// Render triangles

	}

	void RenderPipeline::EndFrame()
	{
		// Render buffer
		HRESULT pres = pSwapChain->Present(1u, 0u);
		DERROR(pres);
	}
}
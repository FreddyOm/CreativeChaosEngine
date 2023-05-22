#include "RenderPipeline.h"
#include "../Analysis/Logger.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace CCE::Graphics
{
	// TODO: Jobify the initialization

	/// <summary>
	/// Initializes the D3D11 
	/// </summary>
	/// <param name="hWnd"></param>
	void RenderPipeline::InitializeD3D11(const HWND hWnd, const int width, const int height)
	{
		LOG_REND("Initializing Direct3D...");

		// TODO: Load from config
		// swap chain
		DXGI_SWAP_CHAIN_DESC _swapChainDesc = {};
		
		_swapChainDesc.BufferDesc.Width = width;							// backbuffer settings
		_swapChainDesc.BufferDesc.Height = height;							// backbuffer settings
		_swapChainDesc.BufferDesc.RefreshRate = DXGI_RATIONAL{ 0,0 };		// backbuffer settings
		_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// backbuffer settings
		_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	// backbuffer settings
		_swapChainDesc.BufferDesc.ScanlineOrdering = 
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;							// backbuffer settings
		_swapChainDesc.SampleDesc.Count = 1;								// sample description
		_swapChainDesc.SampleDesc.Quality = 0;								// sample description
		_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// swap chain is used as back buffer
		_swapChainDesc.BufferCount = 2;										// amount of swap chain buffers
		_swapChainDesc.OutputWindow = hWnd;									// handle to window
		_swapChainDesc.Windowed = TRUE;										// windowed or fullscreen (?)
		_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// swap fx
		_swapChainDesc.Flags = NULL;										// flags
		

		swapChainDesc = _swapChainDesc;

		LOG_REND("Creating Swapchain & Device...");
		// create device and swapchain
		UINT swapCreateFlags = 0u;
#ifdef DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT cdasc = D3D11CreateDeviceAndSwapChain(
			NULL,									// default adapter
			D3D_DRIVER_TYPE_HARDWARE,				// driver type
			NULL,									// software module (Default: NULL)
			swapCreateFlags,						// flags
			nullptr,								// featureLvl
			0,										// length of feature Lvl
			D3D11_SDK_VERSION,						// SDK-Version
			&swapChainDesc,							// swap chain description pointer
			&pSwapChain,							// swap chain pointer
			&pDevice,								// device pointer
			NULL,									// feautre lvl pointer
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

		LOG_REND("Finished initializing Render-Pipeline");
	}

	void RenderPipeline::BeginFrame(const Color col) const
	{
		// Clear render view and draw background color
		pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());
		pContext->ClearRenderTargetView(pRenderTarget.Get(), col.RGBA());
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

		// TODO: Render triangles

	}

	void RenderPipeline::EndFrame()
	{
		// Render buffer
		HRESULT pres = pSwapChain->Present(1u, 0);
		if (pres == DXGI_ERROR_DEVICE_REMOVED)
		{
			DERROR(pDevice->GetDeviceRemovedReason());
		}
		DERROR(pres);
	}
}
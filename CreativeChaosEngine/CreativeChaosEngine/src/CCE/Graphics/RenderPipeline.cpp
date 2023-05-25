#include "RenderPipeline.h"
#include "../Analysis/Logger.h"
#include <dxgi.h>
#include <d3dcompiler.h>
#include <functional>

#pragma comment(lib, "d3d11.lib")
#pragma comment( lib, "dxgi.lib" )
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
		// TODO: Load from config
		// swap chain
		DXGI_SWAP_CHAIN_DESC _swapChainDesc = {0};
		
		_swapChainDesc.BufferDesc.Width = width;							// backbuffer settings
		_swapChainDesc.BufferDesc.Height = height;							// backbuffer settings
		_swapChainDesc.BufferDesc.RefreshRate = DXGI_RATIONAL{ 0,1 };		// backbuffer settings
		_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// backbuffer settings
		_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	// backbuffer settings
		_swapChainDesc.BufferDesc.ScanlineOrdering = 
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;							// backbuffer settings
		_swapChainDesc.SampleDesc.Count = 1;								// sample description
		_swapChainDesc.SampleDesc.Quality = 0;								// sample description
		_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// swap chain is used as back buffer
		_swapChainDesc.BufferCount = 1;										// amount of swap chain buffers
		_swapChainDesc.OutputWindow = hWnd;									// handle to window
		_swapChainDesc.Windowed = TRUE;										// windowed or fullscreen (?)
		_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// swap fx
		_swapChainDesc.Flags = NULL;										// flags
		
		swapChainDesc = _swapChainDesc;

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
			pSwapChain.GetAddressOf(),				// swap chain pointer
			pDevice.GetAddressOf(),					// device pointer
			NULL,									// feautre lvl pointer
			pContext.GetAddressOf()					// context pointer
		);

		DERROR((HRESULT)cdasc);
		DASSERT(cdasc == S_OK, "Creating Device and Swapchain unsuccessful!");

		HRESULT sc = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**) pBackBuffer.GetAddressOf());
		DERROR((HRESULT)sc);
		DASSERT(SUCCEEDED(sc), "Getting buffer was unsuccessful!");

		HRESULT crtv = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRenderTarget.GetAddressOf());
		DERROR((HRESULT)crtv);
		DASSERT(SUCCEEDED(crtv), "Creating render target view was unsuccessful!");

		// check again
		D3D11_DEPTH_STENCIL_DESC dsDesc = {0};
		dsDesc.DepthEnable = TRUE;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

		// creating depth stencil state
		ComPtr<ID3D11DepthStencilState> pDSState;
		DASSERT(SUCCEEDED(pDevice->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf())),
			"Failed creating a depth stencil state!");

		// bind depth state
		pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

		// create depth stencil texture
		ComPtr<ID3D11Texture2D> pDepthStencil;
		D3D11_TEXTURE2D_DESC descDepth = {0};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1u;
		descDepth.ArraySize = 1u;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1u;
		descDepth.SampleDesc.Quality = 0u;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(pDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencil.GetAddressOf()) == S_OK,
			"Failed creating depth stencil texture!");

		// create view of depth stencil tex
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0u;

		DASSERT(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, pDSV.GetAddressOf()) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());

		// configure viewport
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = (float)width;
		vp.Height = (float)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		pContext->RSSetViewports(1u, &vp);
	}

	/// <summary>
	/// Starts the frame by clearing the depth and the stencil buffer.
	/// </summary>
	/// <param name="col">The color of the background.</param>
	void RenderPipeline::BeginFrame(const Color col) const
	{
		// Clear render view and draw background color
		pContext->OMSetRenderTargets(1u, pRenderTarget.GetAddressOf(), pDSV.Get());

#if FALSE
		JobManager::Counter cnt = JobManager::Counter(2);
		//JobManager::EntryPoint cdsvEp = [&JOB_ENTRY_POINT](...) (RenderPipeline::ClearDepthStencilView, this);
		//JobManager::EntryPoint cdsvEp = [&](...){return ClearDepthStencilView(); };
		JobManager::EntryPoint cdsvEp = BIND(ClearDepthStencilView);

		JOBDECL cdsv = JOBDECL(cdsvEp, JobManager::Priority::NORMAL);

		JobManager::EntryPoint crtvEp = BIND(ClearRenderTargetView, col);
		JOBDECL crtv = JOBDECL(crtvEp, JobManager::Priority::NORMAL);

		JobManager::Instance->KickJobAndFreeDecl(cdsv, &cnt);
		JobManager::Instance->KickJobAndFreeDecl(crtv, &cnt);

		JobManager::Instance->WaitForCounter(&cnt, 0);

#else
		pContext->ClearRenderTargetView(pRenderTarget.Get(), col.RGBA());
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
#endif	

		// TODO: Render triangles

	}

	/// <summary>
	/// Ends the frame by presenting the current frame.
	/// </summary>
	void RenderPipeline::EndFrame()
	{
		// Render buffer
		HRESULT pres = pSwapChain->Present(1, 0);
		if (pres == DXGI_ERROR_DEVICE_REMOVED)
		{
			DERROR(pDevice->GetDeviceRemovedReason());
		}
		DERROR(pres);
	}

	JOB_ENTRY_POINT RenderPipeline::ClearRenderTargetView(const Color col) const
	{
		pContext->ClearRenderTargetView(pRenderTarget.Get(), col.RGBA());
	}

	JOB_ENTRY_POINT RenderPipeline::ClearDepthStencilView() const
	{
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}
}
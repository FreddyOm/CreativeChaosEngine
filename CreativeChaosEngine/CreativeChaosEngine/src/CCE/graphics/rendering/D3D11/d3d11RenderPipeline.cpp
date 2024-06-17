#include "d3d11RenderPipeline.h"
#include "../analysis/logger.h"
#include "../manager/application.h"
#include "../manager/memoryManager.h"
#include "../manager/profilingManager.h"
#include "../client-window/client-window.h"
#include "../resources/resource-allocator.h"
#include "../multithreading/scoped-spinlock.h"

namespace CCE::Graphics
{
	// @TODO: Jobify the initialization

	ComPtr<ID3D11Debug> g_pDebug{};
	ComPtr<ID3D11Device> g_pDevice = nullptr;
	ComPtr<IDXGISwapChain> g_pSwapChain = nullptr;
	ComPtr<ID3D11DeviceContext> g_pContext = nullptr;
	ComPtr<ID3D11RenderTargetView> g_pRenderTarget = nullptr;
	ComPtr<ID3D11DepthStencilView> g_pDSV = nullptr;
	ComPtr<ID3D11Resource> g_pBackBuffer = nullptr;
	RECT* g_pClientRect = { 0 };

	// @TODO: Load from config
	DXGI_SWAP_CHAIN_DESC* g_pSwapChainDesc = { 0 };
	D3D11_DEPTH_STENCIL_DESC* g_pDSDesc = { 0 };
	D3D11_DEPTH_STENCIL_VIEW_DESC* g_pDescDSV = { };
	ComPtr<ID3D11Texture2D> g_pDepthStencil = nullptr;
	ComPtr<ID3D11DepthStencilState> g_pDSState = nullptr;

	Camera* g_pViewportCamera = nullptr;
	ECS::Systems::RenderingSystem g_RenderingSystem = ECS::Systems::RenderingSystem();

	RenderPipelineConfig g_RenderPipelineConfig = RenderPipelineConfig();

	SpinLock contextLock;

	int GetRenderTargetWidth()
	{
		return g_pClientRect->right - g_pClientRect->left;
	}

	int GetRenderTargetHeight()
	{
		return g_pClientRect->bottom - g_pClientRect->top;
	}

	/// <summary>
	/// Creating a viewport.
	/// </summary>
	void CreateViewport()
	{
		OPTICK_EVENT();
		// configure viewport

		D3D11_VIEWPORT vp = { 0 };
		vp.Width = (float)g_pClientRect->right;
		vp.Height = (float)g_pClientRect->bottom;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		g_pContext->RSSetViewports(1u, &vp);
	}

	/// <summary>
	/// Creating a depth stencil buffer.
	/// </summary>
	void CreateDepthStencil()
	{
		OPTICK_EVENT();
		g_pDSDesc =
			MemoryManager::Instance->rendMemory.AllocAligned<D3D11_DEPTH_STENCIL_DESC>();
		g_pDSDesc->DepthEnable = TRUE;
		g_pDSDesc->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		g_pDSDesc->DepthFunc = D3D11_COMPARISON_LESS;

		// creating depth stencil state
		DASSERT(SUCCEEDED(g_pDevice->CreateDepthStencilState(g_pDSDesc, g_pDSState.GetAddressOf())),
			"Failed creating a depth stencil state!");

		// bind depth state
		g_pContext->OMSetDepthStencilState(g_pDSState.Get(), 1u);

		// create depth stencil texture
		D3D11_TEXTURE2D_DESC decDepth = {0};
		decDepth.Width = g_pClientRect->right;
		decDepth.Height = g_pClientRect->bottom;
		decDepth.MipLevels = 1u;
		decDepth.ArraySize = 1u;
		decDepth.Format = DXGI_FORMAT_D32_FLOAT;
		decDepth.SampleDesc.Count = 1u;
		decDepth.SampleDesc.Quality = 0u;
		decDepth.Usage = D3D11_USAGE_DEFAULT;
		decDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(g_pDevice->CreateTexture2D(&decDepth, nullptr, g_pDepthStencil.GetAddressOf()) == S_OK,
			"Failed creating depth stencil texture!");

		// create view of depth stencil tex
		g_pDescDSV = MemoryManager::Instance->rendMemory.AllocAligned<D3D11_DEPTH_STENCIL_VIEW_DESC>();

		g_pDescDSV->Format = DXGI_FORMAT_D32_FLOAT;
		g_pDescDSV->ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		g_pDescDSV->Texture2D.MipSlice = 0u;

		DASSERT(g_pDevice->CreateDepthStencilView(g_pDepthStencil.Get(), g_pDescDSV, g_pDSV.GetAddressOf()) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		g_pContext->OMSetRenderTargets(1u, g_pRenderTarget.GetAddressOf(), g_pDSV.Get());
	}

	/// <summary>
	/// Creating a render target view.
	/// </summary>
	void CreateRenderTargetView()
	{
		OPTICK_EVENT();
		HRESULT crtv = g_pDevice->CreateRenderTargetView(g_pBackBuffer.Get(), nullptr, g_pRenderTarget.GetAddressOf());
		DERROR((HRESULT)crtv);
		DASSERT(SUCCEEDED(crtv), "Creating render target view was unsuccessful!");
	}

	/// <summary>
	/// Creating a device and a swap chain.
	/// </summary>
	void CreateDeviceAndSwapChain()
	{
		OPTICK_EVENT();
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
			g_pSwapChainDesc,						// swap chain description pointer
			g_pSwapChain.GetAddressOf(),			// swap chain pointer
			g_pDevice.GetAddressOf(),				// device pointer
			NULL,									// feautre lvl pointer
			g_pContext.GetAddressOf()				// context pointer
		);

		DERROR((HRESULT)cdasc);
		DASSERT(cdasc == S_OK, "Creating Device and Swapchain unsuccessful!");

		HRESULT sc = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)g_pBackBuffer.GetAddressOf());
		DERROR((HRESULT)sc);
		DASSERT(SUCCEEDED(sc), "Getting buffer was unsuccessful!");
	}

	/// <summary>
	/// Create a swap chain description.
	/// </summary>
	/// <param name="hWnd">The host windows handle.</param>
	void CreateSwapChainDesc(const HWND& hWnd)
	{
		OPTICK_EVENT();
		g_pClientRect = MemoryManager::Instance->rendMemory.AllocAligned<RECT>();
		GetClientRect(hWnd, g_pClientRect);

		// swap chain
		g_pSwapChainDesc = MemoryManager::Instance->rendMemory.AllocAligned<DXGI_SWAP_CHAIN_DESC>();

		g_pSwapChainDesc->BufferDesc.Width = g_pClientRect->right;				// backbuffer settings
		g_pSwapChainDesc->BufferDesc.Height = g_pClientRect->bottom;			// backbuffer settings
		g_pSwapChainDesc->BufferDesc.RefreshRate = DXGI_RATIONAL{ 0,1 };		// backbuffer settings
		g_pSwapChainDesc->BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// backbuffer settings
		g_pSwapChainDesc->BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;		// backbuffer settings
		g_pSwapChainDesc->BufferDesc.ScanlineOrdering =
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;								// backbuffer settings
		g_pSwapChainDesc->SampleDesc.Count = 1;									// sample description
		g_pSwapChainDesc->SampleDesc.Quality = 0;								// sample description
		g_pSwapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// swap chain is used as back buffer
		g_pSwapChainDesc->BufferCount = 1;										// amount of swap chain buffers
		g_pSwapChainDesc->OutputWindow = hWnd;									// handle to window
		g_pSwapChainDesc->Windowed = TRUE;										// windowed or fullscreen (?)
		g_pSwapChainDesc->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// swap fx
		g_pSwapChainDesc->Flags = 0;											// flags
	}

	/// <summary>
	/// Initializes the D3D11 render pipeline.
	/// </summary>
	/// <param name="hWnd">The handle to the output window.</param>
	/// <param name="width">The width of the output window.</param>
	/// <param name="height">The height of the output window.</param>
	void InitializeD3D11(const HWND hWnd, const int width, const int height)
	{
		OPTICK_EVENT();

		DASSERT((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

		// @TODO: Load from config file
		g_RenderPipelineConfig.VSync = false;

		//CompileAllShaders();

		// Create swap chain description
		CreateSwapChainDesc(hWnd);

		// Create swapchain and device
		CreateDeviceAndSwapChain();

		// Render target view
		CreateRenderTargetView();

		// Create depth stencil
		CreateDepthStencil();

		// Create viewport
		CreateViewport();

		g_RenderingSystem.StartUp();

		g_pViewportCamera = new Camera();
	}

	/// <summary>
	/// Deinitializes the D3D11 render pipeline.
	/// </summary>
	void DeinitializeD3D11()
	{
		OPTICK_EVENT();
		// @TODO: Prevent this from being called twice (explicit destructor from runtime manager and automatic destructor)
		if (MemoryManager::Instance == nullptr || g_pContext == 0) { return; }
		g_pContext->OMSetRenderTargets(0, NULL, NULL);
		g_pContext->Flush();

		delete g_pViewportCamera;

		// TODO: Check alloc and free order!!
		MemoryManager::Instance->rendMemory.FreeAligned<D3D11_DEPTH_STENCIL_VIEW_DESC>();
		MemoryManager::Instance->rendMemory.FreeAligned<D3D11_DEPTH_STENCIL_DESC>();
		MemoryManager::Instance->rendMemory.FreeAligned<DXGI_SWAP_CHAIN_DESC>();
		MemoryManager::Instance->rendMemory.FreeAligned<RECT>();
	}

	/// <summary>
	/// Recreates resources to match the new host window size.
	/// </summary>
	/// <param name="hWnd">Handle to the host window.</param>
	/// <param name="wParam">WPARAM of the initial message.</param>
	/// <param name="width">The new width.</param>
	/// <param name="height">The new height.</param>
	Jobs::JobReturnType OnResize(const HWND hWnd, const UINT wParam, const int width, const int height)
	{
		OPTICK_EVENT();
		GetClientRect(hWnd, g_pClientRect);

		// Unbind render target and reset resources
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		g_pContext->OMSetRenderTargets(_countof(nullViews), nullViews, NULL);
		g_pRenderTarget.Reset();
		g_pDSV.Reset();
		g_pBackBuffer.Reset();
		g_pDepthStencil.Reset();
		g_pContext->ClearState();
		g_pContext->Flush();

		if (wParam == SIZE_MINIMIZED)
		{
			return;
		}

		// resize buffers
		HRESULT hr = g_pSwapChain->ResizeBuffers((UINT)1, (UINT)g_pClientRect->right, (UINT)g_pClientRect->bottom, DXGI_FORMAT_UNKNOWN, NULL);
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			DASSERT(false, "Resizing buffers failed because device was removed!");
		}
		else if (hr == DXGI_ERROR_DEVICE_RESET)
		{
			DASSERT(false, "Resizing buffers failed because device was reset!");
		}

		hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&g_pBackBuffer);
		DERROR((HRESULT)hr);
		DASSERT(SUCCEEDED(hr), "Getting buffer was unsuccessful!");

		CreateRenderTargetView();

		D3D11_TEXTURE2D_DESC decDepth = { 0 };
		decDepth.Width = g_pClientRect->right;
		decDepth.Height = g_pClientRect->bottom;
		decDepth.MipLevels = 1u;
		decDepth.ArraySize = 1u;
		decDepth.Format = DXGI_FORMAT_D32_FLOAT;
		decDepth.SampleDesc.Count = 1u;
		decDepth.SampleDesc.Quality = 0u;
		decDepth.Usage = D3D11_USAGE_DEFAULT;
		decDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(g_pDevice->CreateTexture2D(&decDepth, nullptr, g_pDepthStencil.GetAddressOf()) == S_OK,
			"Failed creating depth stencil texture!");

		DASSERT(g_pDevice->CreateDepthStencilView(g_pDepthStencil.Get(), g_pDescDSV, g_pDSV.GetAddressOf()) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		g_pContext->OMSetRenderTargets(1u, g_pRenderTarget.GetAddressOf(), g_pDSV.Get());

		CreateViewport();

		if (SUCCEEDED(g_pDevice.As(&g_pDebug)))
		{
			g_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		}
	}

	/// <summary>
	/// The job entry point for creating the reder target view.
	/// </summary>
	/// <param name="col">The background color.</param>
	Jobs::JobReturnType ClearRenderTargetView(uintptr_t col)
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		ScopedSpinLock lock(contextLock);
		g_pContext->ClearRenderTargetView(g_pRenderTarget.Get(), reinterpret_cast<Color*>(col)->RGBA());
	}

	/// <summary>
	/// The job entry point for clearing the depth stencil view.
	/// </summary>
	Jobs::JobReturnType ClearDepthStencilView()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		ScopedSpinLock lock(contextLock);
		g_pContext->ClearDepthStencilView(g_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	Jobs::JobReturnType OMSetRenderTarget()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		ScopedSpinLock lock(contextLock);
		g_pContext->OMSetRenderTargets(1u, g_pRenderTarget.GetAddressOf(), g_pDSV.Get());
	}

	Jobs::JobReturnType UpdateViewportCamera()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		g_pViewportCamera->Update();
	}

	Jobs::JobReturnType UpdateRenderingSystem()
	{
		OPTICK_EVENT();
		SAMPLE_JOB();
		g_RenderingSystem.UpdateSystem();
	}

	/// <summary>
	/// Starts the frame by clearing the depth and the stencil buffer.
	/// </summary>
	/// <param name="pContext"></param>
	/// <param name="col">The color of the background.</param>
	Jobs::JobReturnType BeginFrame(uintptr_t color)
	{
		if (CCE::ClientWindow::Instance->minimized) { return; }
		OPTICK_EVENT();
		SAMPLE_JOB();

		// Clear render view and draw background color
		
		//Jobs::Counter cnt = Jobs::Counter(5);

		//Jobs::Job beginFrameJobs[5]
		//{
		//	// Create job descriptions
		//	Jobs::JOB(ClearDepthStencilView, &cnt, Jobs::Priority::CRITICAL),
		//	Jobs::JOB(ClearRenderTargetView, &cnt, Jobs::Priority::CRITICAL, color),
		//	Jobs::JOB(OMSetRenderTarget, &cnt, Jobs::Priority::HIGH),
		//	Jobs::JOB(UpdateViewportCamera, &cnt, Jobs::Priority::NORMAL),
		//	Jobs::JOB(UpdateRenderingSystem, &cnt, Jobs::Priority::NORMAL),
		//};

		//Jobs::KickJobs(&beginFrameJobs[0], 5);

		//// Await execution
		//Jobs::BusyWaitForCounter(&cnt);		

		ClearDepthStencilView();
		ClearRenderTargetView(color);

		OMSetRenderTarget();

		UpdateViewportCamera();
		UpdateRenderingSystem();
	}

	/// <summary>
	/// Ends the frame by presenting the backbuffer.
	/// </summary>
	Jobs::JobReturnType EndFrame()
	{
		if (ClientWindow::Instance->minimized) { return; }
		OPTICK_EVENT();
		SAMPLE_JOB();
		// Render buffer

		HRESULT pres = g_pSwapChain->Present(g_RenderPipelineConfig.VSync ? 1 : 0, 0);
		
#if defined(DEBUG) || defined(DEBUG_PROFILE)

		if (pres == DXGI_ERROR_DEVICE_REMOVED)
		{
			DERROR(g_pDevice->GetDeviceRemovedReason());
		}
		DERROR(pres);
#endif
	}
}

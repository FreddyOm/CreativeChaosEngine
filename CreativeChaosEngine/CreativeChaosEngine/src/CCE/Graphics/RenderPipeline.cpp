#include "RenderPipeline.h"
#include "../Analysis/Logger.h"
#include "../Manager/MemoryManager.h"
#include "../ClientWindow/ClientWindow.h"
#include <functional>
#include "Rendering/Drawable/Mesh.h"
#include "Rendering/Camera.h"

namespace CCE::Graphics
{
	// TODO: Jobify the initialization

	RenderPipeline* RenderPipeline::Instance = nullptr;

	RenderPipeline::~RenderPipeline()
	{
		UninitializeD3D11();

		p_renderTarget.Reset();
		p_DSV.Reset();
		p_backBuffer.Reset();
		pSwapChain.Reset();
		p_Context.Reset();
		p_device.Reset();

		p_device.~ComPtr();
		pSwapChain.~ComPtr();
		p_Context.~ComPtr();
		p_renderTarget.~ComPtr();
		p_DSV.~ComPtr();
		p_backBuffer.~ComPtr();

		//TODO: Check this bug when deleting cnt
		//if (cnt != nullptr)
			//delete cnt;

		//delete viewportCamera;
		delete testMesh;

		UNREGISTER_LEAK_DETECT;
	}

	/// <summary>
	/// Initializes the D3D11 
	/// </summary>
	/// <param name="hWnd"></param>
	void RenderPipeline::InitializeD3D11(const HWND hWnd, const int width, const int height)
	{
		// TODO: Load from config file
		pipelineConfig.VSync = false;

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

		//viewportCamera = new Camera();
		testMesh = new Mesh();
	}

	/// <summary>
	/// Creating a viewport.
	/// </summary>
	void RenderPipeline::CreateViewport()
	{
		// configure viewport

		D3D11_VIEWPORT vp = { 0 };
		vp.Width = (float)clientRect->right;
		vp.Height = (float)clientRect->bottom;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		p_Context->RSSetViewports(1u, &vp);
	}

	/// <summary>
	/// Creating a depth stencil buffer.
	/// </summary>
	void RenderPipeline::CreateDepthStencil()
	{
		dsDesc =
			MemoryManager::Instance->rendMemory.AllocAligned<D3D11_DEPTH_STENCIL_DESC>();
		dsDesc->DepthEnable = TRUE;
		dsDesc->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc->DepthFunc = D3D11_COMPARISON_LESS;

		// creating depth stencil state
		DASSERT(SUCCEEDED(p_device->CreateDepthStencilState(dsDesc, pDSState.GetAddressOf())),
			"Failed creating a depth stencil state!");

		// bind depth state
		p_Context->OMSetDepthStencilState(pDSState.Get(), 1u);

		// create depth stencil texture
		D3D11_TEXTURE2D_DESC decDepth = {0};
		decDepth.Width = clientRect->right;
		decDepth.Height = clientRect->bottom;
		decDepth.MipLevels = 1u;
		decDepth.ArraySize = 1u;
		decDepth.Format = DXGI_FORMAT_D32_FLOAT;
		decDepth.SampleDesc.Count = 1u;
		decDepth.SampleDesc.Quality = 0u;
		decDepth.Usage = D3D11_USAGE_DEFAULT;
		decDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(p_device->CreateTexture2D(&decDepth, nullptr, pDepthStencil.GetAddressOf()) == S_OK,
			"Failed creating depth stencil texture!");

		// create view of depth stencil tex
		descDSV = MemoryManager::Instance->rendMemory.AllocAligned<D3D11_DEPTH_STENCIL_VIEW_DESC>();

		descDSV->Format = DXGI_FORMAT_D32_FLOAT;
		descDSV->ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV->Texture2D.MipSlice = 0u;

		DASSERT(p_device->CreateDepthStencilView(pDepthStencil.Get(), descDSV, p_DSV.GetAddressOf()) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		p_Context->OMSetRenderTargets(1u, p_renderTarget.GetAddressOf(), p_DSV.Get());
	}

	/// <summary>
	/// Creating a render target view.
	/// </summary>
	void RenderPipeline::CreateRenderTargetView()
	{
		HRESULT crtv = p_device->CreateRenderTargetView(p_backBuffer.Get(), nullptr, p_renderTarget.GetAddressOf());
		DERROR((HRESULT)crtv);
		DASSERT(SUCCEEDED(crtv), "Creating render target view was unsuccessful!");
	}

	/// <summary>
	/// Creating a device and a swap chain.
	/// </summary>
	void RenderPipeline::CreateDeviceAndSwapChain()
	{
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
			swapChainDesc,							// swap chain description pointer
			pSwapChain.GetAddressOf(),				// swap chain pointer
			p_device.GetAddressOf(),				// device pointer
			NULL,									// feautre lvl pointer
			p_Context.GetAddressOf()				// context pointer
		);

		DERROR((HRESULT)cdasc);
		DASSERT(cdasc == S_OK, "Creating Device and Swapchain unsuccessful!");

		HRESULT sc = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)p_backBuffer.GetAddressOf());
		DERROR((HRESULT)sc);
		DASSERT(SUCCEEDED(sc), "Getting buffer was unsuccessful!");
	}

	/// <summary>
	/// Create a swap chain description.
	/// </summary>
	/// <param name="hWnd">The host windows handle.</param>
	void RenderPipeline::CreateSwapChainDesc(const HWND& hWnd)
	{
		clientRect = MemoryManager::Instance->rendMemory.AllocAligned<RECT>();
		GetClientRect(hWnd, clientRect);

		// swap chain
		swapChainDesc = MemoryManager::Instance->rendMemory.AllocAligned<DXGI_SWAP_CHAIN_DESC>();

		swapChainDesc->BufferDesc.Width = clientRect->right;					// backbuffer settings
		swapChainDesc->BufferDesc.Height = clientRect->bottom;				// backbuffer settings
		swapChainDesc->BufferDesc.RefreshRate = DXGI_RATIONAL{ 0,1 };		// backbuffer settings
		swapChainDesc->BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;		// backbuffer settings
		swapChainDesc->BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;		// backbuffer settings
		swapChainDesc->BufferDesc.ScanlineOrdering =
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;							// backbuffer settings
		swapChainDesc->SampleDesc.Count = 1;								// sample description
		swapChainDesc->SampleDesc.Quality = 0;								// sample description
		swapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// swap chain is used as back buffer
		swapChainDesc->BufferCount = 1;										// amount of swap chain buffers
		swapChainDesc->OutputWindow = hWnd;									// handle to window
		swapChainDesc->Windowed = TRUE;										// windowed or fullscreen (?)
		swapChainDesc->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;				// swap fx
		swapChainDesc->Flags = 0;											// flags
	}


	/// <summary>
	/// Starts the frame by clearing the depth and the stencil buffer.
	/// </summary>
	/// <param name="col">The color of the background.</param>
	void RenderPipeline::BeginFrame(const Color col)
	{
		if ( ClientWindow::Instance->minimized) { return; }

		// Clear render view and draw background color
		p_Context->ClearRenderTargetView(p_renderTarget.Get(), col.RGBA());
		p_Context->ClearDepthStencilView(p_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
		
#if MULTITHREADED
		*cnt = 2;

		JobManager::EntryPoint cdsvEp = BIND(RenderPipeline::Instance->ClearDepthStencilView);
		JOBDECL cdsv = JOBDECL(cdsvEp, JobManager::Priority::HIGH);

		JobManager::EntryPoint crtvEp = BIND(RenderPipeline::Instance->ClearRenderTargetView, col);
		JOBDECL crtv = JOBDECL(crtvEp, JobManager::Priority::NORMAL);

		JobManager::Instance->KickJobAndFreeDecl(crtv, cnt);

		JobManager::Instance->WaitForCounter(*cnt, 1);

		JobManager::Instance->KickJobAndFreeDecl(cdsv, cnt);

		JobManager::Instance->WaitForCounter(*cnt, 0);

#else
		p_Context->OMSetRenderTargets(1u, p_renderTarget.GetAddressOf(), p_DSV.Get());
#endif	

		// TODO: Render triangles

		// Test Cube

		//viewportCamera->Update();

		testMesh->Draw();
	}

	/// <summary>
	/// Ends the frame by presenting the current frame.
	/// </summary>
	void RenderPipeline::EndFrame()
	{
		if (ClientWindow::Instance->minimized) { return; }
	
		// Render buffer
		HRESULT pres = pSwapChain->Present(pipelineConfig.VSync ? 1 : 0, 0);
		if (pres == DXGI_ERROR_DEVICE_REMOVED)
		{
			DERROR(p_device->GetDeviceRemovedReason());
		}
		DERROR(pres);
	}

	/// <summary>
	/// Recreates resources to match the new host window size.
	/// </summary>
	/// <param name="hWnd">Handle to the host window.</param>
	/// <param name="wParam">WPARAM of the initial message.</param>
	/// <param name="width">The new width.</param>
	/// <param name="height">The new height.</param>
	void RenderPipeline::OnResize(const HWND hWnd, const UINT wParam, const int width, const int height)
	{
		//if (p_Context == NULL) { return; }

		GetClientRect(hWnd, clientRect);

		// Unbind render target and reset resources
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		p_Context->OMSetRenderTargets(_countof(nullViews), nullViews, NULL);
		p_renderTarget.Reset();
		p_DSV.Reset();
		p_backBuffer.Reset();
		pDepthStencil.Reset();
		p_Context->ClearState();
		p_Context->Flush();
		
		if (wParam == SIZE_MINIMIZED)
		{
			return;
		}

		// resize buffers
		HRESULT hr = pSwapChain->ResizeBuffers((UINT)1, (UINT)clientRect->right, (UINT)clientRect->bottom, DXGI_FORMAT_UNKNOWN, NULL);
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			DASSERT(false, "Resizing buffers failed because device was removed!");
		}
		else if (hr == DXGI_ERROR_DEVICE_RESET)
		{
			DASSERT(false, "Resizing buffers failed because device was reset!");
		}
		
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &p_backBuffer);
		DERROR((HRESULT)hr);
		DASSERT(SUCCEEDED(hr), "Getting buffer was unsuccessful!");

		CreateRenderTargetView();

		D3D11_TEXTURE2D_DESC decDepth = { 0 };
		decDepth.Width = clientRect->right;
		decDepth.Height = clientRect->bottom;
		decDepth.MipLevels = 1u;
		decDepth.ArraySize = 1u;
		decDepth.Format = DXGI_FORMAT_D32_FLOAT;
		decDepth.SampleDesc.Count = 1u;
		decDepth.SampleDesc.Quality = 0u;
		decDepth.Usage = D3D11_USAGE_DEFAULT;
		decDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		DASSERT(p_device->CreateTexture2D(&decDepth, nullptr, pDepthStencil.GetAddressOf()) == S_OK,
			"Failed creating depth stencil texture!");

		DASSERT(p_device->CreateDepthStencilView(pDepthStencil.Get(), descDSV, p_DSV.GetAddressOf()) == S_OK,
			"Failed creating depth stencil view");

		// bind depth stensil view to OM
		p_Context->OMSetRenderTargets(1u, p_renderTarget.GetAddressOf(), p_DSV.Get());

		CreateViewport();

		if (SUCCEEDED(p_device.As(&pDebug)))
		{
			pDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
		}
	}

	/// <summary>
	/// Releases all preallocated memory for the rendering.
	/// </summary>
	void RenderPipeline::UninitializeD3D11()
	{
		// TODO: Prevent this from being called twice (explicit destructor from runtime manager and automatic destructor)
		if (MemoryManager::Instance == nullptr || p_Context == 0) { return; }
		p_Context->OMSetRenderTargets(0, NULL, NULL);
		p_Context->Flush();

		// TODO: Check alloc and free order!!
		MemoryManager::Instance->rendMemory.FreeAligned(sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		MemoryManager::Instance->rendMemory.FreeAligned(sizeof(D3D11_DEPTH_STENCIL_DESC));
		MemoryManager::Instance->rendMemory.FreeAligned(sizeof(DXGI_SWAP_CHAIN_DESC));
		MemoryManager::Instance->rendMemory.FreeAligned(sizeof(RECT));
	}

	/// <summary>
	/// Compile all shaders.
	/// </summary>
	/// <returns>True if successful.</returns>
	bool RenderPipeline::CompileAllShaders() noexcept
	{
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif
		ComPtr<ID3DBlob> vs_blob_ptr = NULL,  ps_blob_ptr = NULL, err_blob_ptr = NULL;

		// COMPILE VERTEX SHADER
		HRESULT hr = D3DCompileFromFile(
			L"resources/shader/DefaultVertexShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			flags,
			0,
			&vs_blob_ptr,
			&err_blob_ptr);

		if (FAILED(hr)) {
			if (err_blob_ptr) { DERROR((char*)err_blob_ptr->GetBufferPointer()); }
			DERROR(hr);
			if (vs_blob_ptr) { vs_blob_ptr->Release(); }
			//assert(false);
		}

		// COMPILE PIXEL SHADER
		hr = D3DCompileFromFile(
			L"resources/shader/DefaultPixelShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ps_5_0",
			flags,
			0,
			&ps_blob_ptr,
			&err_blob_ptr);

		if (FAILED(hr)) {
			if (err_blob_ptr) { DERROR((char*)err_blob_ptr->GetBufferPointer()); }
			DERROR(hr);
			if (ps_blob_ptr) { ps_blob_ptr->Release(); }
			//assert(false);
		}

		return false;
	}

#pragma region Job System Entry Points

	/// <summary>
	/// The job entry point for creating the reder target view.
	/// </summary>
	/// <param name="col">The background color.</param>
	JOB_ENTRY_POINT RenderPipeline::ClearRenderTargetView(Color col) const
	{
		p_Context->ClearRenderTargetView(p_renderTarget.Get(), col.RGBA());
	}

	/// <summary>
	/// The job entry point for clearing the depth stencil view.
	/// </summary>
	JOB_ENTRY_POINT RenderPipeline::ClearDepthStencilView() const
	{
		p_Context->ClearDepthStencilView(p_DSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

#pragma endregion

}

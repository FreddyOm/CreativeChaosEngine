#include "RenderManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "../Analysis/Time.h"

#pragma comment(lib,"d3d11.lib")

namespace CCE
{
	void RenderManager::StartUp()
	{
		DASSERT(Instance == nullptr, "RenderManager was instantiated more than once!");
		Instance = this;

		auto startTime = Time::CurrentTick();
		initialized = true;

		auto endTime = Time::CurrentTick();
		double initDuration = Time::GetDurationInMicroSec(startTime, endTime);
		LOGC("RenderManager initialized!", COLOR_BLUE);
	}

	void RenderManager::ShutDown()
	{
		LOGC("Shutting down RenderManager...", COLOR_BLUE);
		initialized = false;

		pDevice.~ComPtr();
		pSwapChain.~ComPtr();
		pContext.~ComPtr();
		pRenderTarget.~ComPtr();
		pBackBuffer.~ComPtr();

		Instance = nullptr;
	}
	
	void RenderManager::InitializeD3D11(const HWND hWnd)
	{
		// TODO: Load from config
		// swap chain
		const DXGI_SWAP_CHAIN_DESC _swapChainDesc =
		{
			dxgiModeDesc,						// backbuffer settings
			dxgiSampleDesc,						// sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,	// swap chain is used as back buffer
			1,									// amount of swap chain buffers
			hWnd,								// handle to window
			TRUE,								// windowed or fullscreen (?)
			DXGI_SWAP_EFFECT_DISCARD,			// swap fx
			NULL								// flags
		};

		swapChainDesc = _swapChainDesc;

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
		
		HRESULT crtv = pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pRenderTarget);
		DERROR((HRESULT)crtv);
		DASSERT(crtv == S_OK, "Creating render target view was unsuccessful!");
	}

	RenderManager* RenderManager::Instance = nullptr;
}

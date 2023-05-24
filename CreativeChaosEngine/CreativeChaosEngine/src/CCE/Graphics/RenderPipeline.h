#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "../Memory/StackAllocator.h"
#include "../Utilities/Color/Color.h"
#include "../Manager/JobManager.h"

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

	public:
		ID3D11Device* GetDevicePtr() const
		{
			return pDevice.Get();
		}

		ID3D11DeviceContext* GetDeviceContextPtr() const
		{
			return pContext.Get();
		}

	private:
		JOB_ENTRY_POINT ClearRenderTargetView(const Color col) const;
		JOB_ENTRY_POINT ClearDepthStencilView() const;

	private:
		ComPtr<ID3D11Device> pDevice = nullptr;
		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		ComPtr<ID3D11DeviceContext> pContext = nullptr;
		ComPtr<ID3D11RenderTargetView> pRenderTarget = nullptr;
		ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
		ComPtr<ID3D11Resource> pBackBuffer = nullptr;

		// TODO: Load from config
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	};
}

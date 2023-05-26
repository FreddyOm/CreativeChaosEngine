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
			p_Context->Flush();
			p_renderTarget->Release();
			p_device->Release();
			pSwapChain->Release();
			p_Context->Release();
			p_backBuffer->Release();
			p_DSV->Release();

			p_device.~ComPtr();
			pSwapChain.~ComPtr();
			p_Context.~ComPtr();
			p_renderTarget.~ComPtr();
			p_DSV.~ComPtr();
			p_backBuffer.~ComPtr();
		}

	public:
		void InitializeD3D11(const HWND hWnd, const int width, const int height);
		void CreateViewport();
		void CreateDepthStencil();
		void CreateRenderTargetView();
		void CreateDeviceAndSwapChain();
		void CreateSwapChainDesc(const HWND& hWnd);
		void BeginFrame(const Color col) const;
		void EndFrame();
		void OnResize(const HWND hWnd, const UINT wParam, const int width, const int height);

	public:
		ID3D11Device* GetDevicePtr() const
		{
			return p_device.Get();
		}

		ID3D11DeviceContext* GetDeviceContextPtr() const
		{
			return p_Context.Get();
		}

	private:
		JOB_ENTRY_POINT ClearRenderTargetView(const Color col) const;
		JOB_ENTRY_POINT ClearDepthStencilView() const;

	private:
		ComPtr<ID3D11Device> p_device = nullptr;
		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		ComPtr<ID3D11DeviceContext> p_Context = nullptr;
		ComPtr<ID3D11RenderTargetView> p_renderTarget = nullptr;
		ComPtr<ID3D11DepthStencilView> p_DSV = nullptr;
		ComPtr<ID3D11Resource> p_backBuffer = nullptr;
		RECT clientRect;

		// TODO: Load from config
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	};
}

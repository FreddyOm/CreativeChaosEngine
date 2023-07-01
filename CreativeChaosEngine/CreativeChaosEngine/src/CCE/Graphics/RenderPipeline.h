#pragma once

#include "../Memory/StackAllocator.h"
#include "../Utilities/Color/Color.h"
#include "../Manager/JobManager.h"
#include "Rendering/D3D11.h"

namespace CCE::Graphics
{
	using Microsoft::WRL::ComPtr;
	using CCE::Color;

	class CCE_API RenderPipeline
	{
	public:

		struct RenderPipelineConfig
		{
			bool activateVSync = true;
			CCE::Color backgroundColor = CCE::Color("#BCC5CE");
		};
		
		RenderPipeline()
		{
			DASSERT(Instance == nullptr, "The Renderpipeline can only be initialized once!");
			Instance = this;
		}
		
		~RenderPipeline()
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
		}

	public:
		static RenderPipeline* Instance;

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
		void UninitializeD3D11();

	public:
		ID3D11Device* GetDevicePtr() const
		{
			return p_device.Get();
		}

		ID3D11DeviceContext* GetDeviceContextPtr() const
		{
			return p_Context.Get();
		}
		
		RenderPipelineConfig* GetRenderPipelineConfig()
		{
			return &pipelineConfig;
		}
		
	private:
		JOB_ENTRY_POINT ClearRenderTargetView(const Color col) const;
		JOB_ENTRY_POINT ClearDepthStencilView() const;

	private:
		ComPtr<ID3D11Debug> pDebug;
		RenderPipelineConfig pipelineConfig;

	private:
		ComPtr<ID3D11Device> p_device = nullptr;
		ComPtr<IDXGISwapChain> pSwapChain = nullptr;
		ComPtr<ID3D11DeviceContext> p_Context = nullptr;
		ComPtr<ID3D11RenderTargetView> p_renderTarget = nullptr;
		ComPtr<ID3D11DepthStencilView> p_DSV = nullptr;
		ComPtr<ID3D11Resource> p_backBuffer = nullptr;
		RECT* clientRect;

		// TODO: Load from config
		DXGI_SWAP_CHAIN_DESC* swapChainDesc = { 0 };
		D3D11_DEPTH_STENCIL_DESC* dsDesc = { 0 };
		D3D11_DEPTH_STENCIL_VIEW_DESC* descDSV = { };
		ComPtr<ID3D11Texture2D> pDepthStencil = nullptr;
		ComPtr<ID3D11DepthStencilState> pDSState = nullptr;
	};
}

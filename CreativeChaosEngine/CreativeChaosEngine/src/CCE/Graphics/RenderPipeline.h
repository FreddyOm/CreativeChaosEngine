#pragma once

#include "Rendering/Camera.h"
#include "../Manager/JobManager.h"
#include "Rendering/Drawable/Model.h"
#include "../Utilities/Color/Color.h"
#include "../Memory/StackAllocator.h"
#include "../Manager/ProfilingManager.h"
#include "../Utilities/Serialization/ISerializable.h"

namespace CCE::Graphics
{
	using CCE::Color;
	struct Model;

	JOB_ENTRY_POINT BeginFrame(ID3D11DeviceContext* pContext,
		ComPtr<ID3D11RenderTargetView>& p_renderTarget,
		ID3D11DepthStencilView* pDSV, CCE::Graphics::Camera* pViewportCamera,
		std::vector<CCE::Graphics::Model*>& testModels, 
		const CCE::Graphics::Color col);

	JOB_ENTRY_POINT ClearRenderTargetView(ID3D11DeviceContext* pContext,
		ComPtr<ID3D11RenderTargetView>& p_renderTarget, Color col);

	JOB_ENTRY_POINT ClearDepthStencilView(ID3D11DeviceContext* pContext, 
		ID3D11DepthStencilView* pDSV);

	class CCE_API RenderPipeline
	{
	public:

		enum class WindowMode
		{
			WINDOW = 0,
			BORDERLESS_WINDOW = 1,
			FULLSCREEN = 2,
		};

		struct RenderPipelineConfig : ISerializable<RenderPipelineConfig>
		{
			bool VSync = true;
			Color backgroundColor = Color("#DEEFE7"); //#BCC5CE
			WindowMode windowMode = WindowMode::WINDOW;

			/// <summary>
			/// Serialize an object to a serialize string.
			/// </summary>
			/// <param name="prettyPrint"></param>
			/// <returns></returns>
			std::string SerializeToString(bool prettyPrint = false) override
			{
				JSON data;

				SERIALIZE_CLASS_MEMBER(VSync);
				data[typeid(*this).name()]["backgroundColor"] = {
					backgroundColor.rgba[0], 
					backgroundColor.rgba[1], 
					backgroundColor.rgba[2], 
					backgroundColor.rgba[3] };
				SERIALIZE_CLASS_MEMBER(windowMode);

				std::string out = prettyPrint ? data.dump(4).c_str() : data.dump().c_str();
				//String out2 = String(_strdup(data.dump(4).c_str()));	// Does this introduce an new invalid string to the string table?
				return out;
			}

			std::vector<uint8_t>SerializeToBinary() override
			{
				JSON data;

				SERIALIZE_CLASS_MEMBER(VSync);
				data[typeid(*this).name()]["backgroundColor"] = {
					backgroundColor.rgba[0],
					backgroundColor.rgba[1],
					backgroundColor.rgba[2],
					backgroundColor.rgba[3] };
				SERIALIZE_CLASS_MEMBER(windowMode);

				return JSON::to_bson(data);
			}

			/// <summary>
			/// Deserialize and set the values from a serialized string.
			/// </summary>
			/// <param name="serializeString"></param>
			void DeserializeFromString(std::string serializeString) override
			{
				JSON data = JSON::parse(serializeString);

				DESERIALIZE_CLASS_MEMBER(VSync);
				backgroundColor = Color(
					(float)data[typeid(*this).name()]["backgroundColor"][0],
					(float)data[typeid(*this).name()]["backgroundColor"][1],
					(float)data[typeid(*this).name()]["backgroundColor"][2],
					(float)data[typeid(*this).name()]["backgroundColor"][3]);
				DESERIALIZE_CLASS_MEMBER(windowMode);
			}

			/// <summary>
			/// Deserialize and set the values from a serialized binary.
			/// </summary>
			/// <param name="serializeData"></param>
			void DeserializeFromBinary(std::vector<uint8_t> serializeData) override
			{
				JSON data = JSON::from_bson(serializeData);

				DESERIALIZE_CLASS_MEMBER(VSync);
				backgroundColor = Color(
					(float)data[typeid(*this).name()]["backgroundColor"][0],
					(float)data[typeid(*this).name()]["backgroundColor"][1],
					(float)data[typeid(*this).name()]["backgroundColor"][2],
					(float)data[typeid(*this).name()]["backgroundColor"][3]);
				DESERIALIZE_CLASS_MEMBER(windowMode);
			}
		};
		
		RenderPipeline()
		{
			DASSERT(Instance == nullptr, "The Renderpipeline can only be initialized once!");
			Instance = this;
			DASSERT((sizeof(ConstantBufferStruct) % 16) == 0, "Constant Buffer size must be 16-byte aligned");
			REGISTER_LEAK_DETECT;
		}
		
		~RenderPipeline();
		
		typedef struct _constantBufferStruct {
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
		} ConstantBufferStruct;

	public:
		static RenderPipeline* Instance;
		Camera* pViewportCamera = nullptr;
		std::vector<Model*> testModels = { };

	public:
		void InitializeD3D11(const HWND hWnd, const int width, const int height);
		void CreateViewport();
		void CreateDepthStencil();
		void CreateRenderTargetView();
		void CreateDeviceAndSwapChain();
		void CreateSwapChainDesc(const HWND& hWnd);
		void BeginFrame(const Color col);
		void EndFrame();
		void OnResize(const HWND hWnd, const UINT wParam, const int width, const int height);
		void UninitializeD3D11();

		bool CompileAllShaders() noexcept;

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

		ID3D11RenderTargetView* GetRenderTargetPtr() const
		{
			return p_renderTarget.Get();
		}
		
		ComPtr<ID3D11RenderTargetView> GetRenderTargetComPtr() const
		{
			return p_renderTarget;
		}

		ID3D11DepthStencilView* GetDepthStencilViewPtr() const
		{
			return p_DSV.Get();
		}

		int GetRenderTargetWidth()
		{
			return clientRect->right - clientRect->left;
		}

		int GetRenderTargetHeight()
		{
			return clientRect->bottom - clientRect->top;
		}

	public:
		JOB_ENTRY_POINT ClearDepthStencilView() const;
		JobManager::Counter cnt = JobManager::Counter(2);

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
		RECT* clientRect = { 0 };

		// TODO: Load from config
		DXGI_SWAP_CHAIN_DESC* swapChainDesc = { 0 };
		D3D11_DEPTH_STENCIL_DESC* dsDesc = { 0 };
		D3D11_DEPTH_STENCIL_VIEW_DESC* descDSV = { };
		ComPtr<ID3D11Texture2D> pDepthStencil = nullptr;
		ComPtr<ID3D11DepthStencilState> pDSState = nullptr;
	};
}

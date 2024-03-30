#pragma once
#include "../core.h"
#include "../multithreading/job-system.h"

#include "../utilities/Serialization/ISerializable.h"
#include "../utilities/color/color.h"
#include "../ecs/systems/rendering-system.h"

#include "rendering/drawable/model.h"
#include "rendering/camera.h"
#include "rendering/D3D11.h"
#include <vector>

namespace CCE::Graphics
{
	typedef struct _constantBufferStruct {
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	} ConstantBufferStruct;

	struct RenderPipelineConfig : ISerializable<RenderPipelineConfig>
	{
		enum class WindowMode
		{
			WINDOW = 0,
			BORDERLESS_WINDOW = 1,
			FULLSCREEN = 2,
		};

		bool VSync = true;
		Color backgroundColor = Color("#DEEFE7"); //#BCC5CE
		WindowMode windowMode = WindowMode::WINDOW;

		/// <summary>
		/// Serialize an object to a serialize string.
		/// </summary>
		/// <param name="prettyPrint"></param>
		/// <returns></returns>
		std::string SerializeToString(bool prettyPrint = false) const override
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

		std::vector<uint8_t>SerializeToBinary() const override
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

	struct Model;

	// Globals

	extern CCE_API ComPtr<ID3D11DeviceContext> g_pContext;
	extern CCE_API ComPtr<ID3D11Device> g_pDevice;
	extern ComPtr<IDXGISwapChain> g_pSwapChain;
	extern ComPtr<ID3D11RenderTargetView> g_pRenderTarget;
	extern ComPtr<ID3D11DepthStencilView> g_pDSV;
	extern ComPtr<ID3D11Resource> g_pBackBuffer;
	extern RECT* g_pClientRect;

	extern ComPtr<ID3D11Debug> g_pDebug;

	// @TODO: Load from config
	extern DXGI_SWAP_CHAIN_DESC* g_pSwapChainDesc;
	extern D3D11_DEPTH_STENCIL_DESC* g_pDSDesc;
	extern D3D11_DEPTH_STENCIL_VIEW_DESC* g_pDescDSV;
	extern ComPtr<ID3D11Texture2D> g_pDepthStencil;
	extern ComPtr<ID3D11DepthStencilState> g_pDSState;

	extern CCE_API Camera* g_pViewportCamera;
	extern ECS::Systems::RenderingSystem g_RenderingSystem;

	extern CCE_API RenderPipelineConfig g_RenderPipelineConfig;

	extern void InitializeD3D11(const HWND hWnd, const int width, const int height);
	extern void DeinitializeD3D11();
	extern Jobs::JobReturnType OnResize(const HWND hWnd, const UINT wParam, const int width, const int height);

	extern Jobs::JobReturnType BeginFrame(uintptr_t col);
	extern Jobs::JobReturnType EndFrame();

	extern int GetRenderTargetWidth();
	extern int GetRenderTargetHeight();
}

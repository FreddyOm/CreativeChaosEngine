#include "Camera.h"
#include "../Rendering/../../Manager/ProfilingManager.h"
#include "../RenderPipeline.h"

namespace CCE::Graphics
{
	Camera::Camera()
	{

		transform.SetPosition({0.0f, 0.0f, -2.0f});
		SetFovAndLookDir();
		CreateConstBufs();

		REGISTER_LEAK_DETECT;
	}

	Camera::~Camera()
	{
		UNREGISTER_LEAK_DETECT;
	}

#pragma region setter

	void Camera::SetProjection(float _near, float _far, float _fovV, float _fovH) noexcept
	{
		nearPlane = _near;
		farPlane = _far;
		fovVertical = _fovV;
		fovHorizontal = _fovH;
	}

#pragma endregion setter

#pragma region getter

	const float Camera::GetNearPlane() const noexcept
	{
		return nearPlane;
	}

	const float Camera::GetFarPlane() const noexcept
	{
		return farPlane;
	}

	const float Camera::GetVerticalFOV() const noexcept
	{
		return fovVertical;
	}

	const float Camera::GetHorizontalFOV() const noexcept
	{
		return fovHorizontal;
	}

#pragma endregion getter

	void Camera::CreateConstBufs()
	{
		HRESULT hr;
		// Const bufs
		D3D11_BUFFER_DESC desc{};
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(DirectX::XMMATRIX);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		hr = RenderPipeline::Instance->GetDevicePtr()->CreateBuffer(&desc, nullptr, &pPerFrameConstBuf);

		if (FAILED(hr)) { DERROR(hr); }
	}

	// TODO: Only do this when necessary!
	void Camera::SetFovAndLookDir() noexcept
	{
		using namespace DirectX;

		ZeroMemory(&viewProjectionMatrix, sizeof(viewProjectionMatrix));

		float fovRadians = (fovVertical / 360.0f) * XM_2PI;

		XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(fovRadians,
			static_cast<float>(RenderPipeline::Instance->GetRenderTargetWidth()) /
			static_cast<float>(RenderPipeline::Instance->GetRenderTargetHeight()),
			0.1f,
			100.0f);
		
		 XMMATRIX view = XMMatrixLookAtLH(
			{ transform.Position().x, transform.Position().y, transform.Position().z },
			 { transform.Position().x, transform.Position().y, transform.Position().z + 1 }, 
			 {0,1,0,0});
		
		viewProjectionMatrix = view * projMatrix;
	}

	void Camera::Update()
	{
		SetFovAndLookDir();

		HRESULT hr;

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		hr = RenderPipeline::Instance->GetDeviceContextPtr()->Map(pPerFrameConstBuf.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		
		if (FAILED(hr)) { DERROR(hr); }
		
		memcpy(mappedResource.pData, &viewProjectionMatrix, sizeof(DirectX::XMMATRIX));
		RenderPipeline::Instance->GetDeviceContextPtr()->Unmap(pPerFrameConstBuf.Get(), 0);

		RenderPipeline::Instance->GetDeviceContextPtr()->VSSetConstantBuffers(0, 1, pPerFrameConstBuf.GetAddressOf());
	}
}

#pragma once
#include "../../Core.h"
#include "D3D11.h"
#include "Transform.h"

namespace CCE::Graphics
{

	struct CCE_API Camera
	{
	public:
		Camera();
		~Camera();

		Transform transform;

		void SetProjection(float _near, float _far, float _fovV, float _fovH) noexcept;

		const DirectX::XMMATRIX GetViewProjectionMatrix() const noexcept
		{ return DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix); }
		
		const float GetNearPlane() const noexcept;
		const float GetFarPlane() const noexcept;
		const float GetVerticalFOV() const noexcept;
		const float GetHorizontalFOV() const noexcept;

		void Update();

	private:
		float nearPlane = 0.1f;
		float farPlane = 20.0f;
		float fovVertical = 120.0f;
		float fovHorizontal = 160.0f;

		DirectX::XMMATRIX projectionMatrix;
		DirectX::XMMATRIX viewMatrix;

		//ComPtr<ConstantBuffer<DirectX::XMMATRIX>> pPerFrameConstBuf = nullptr;
		//ComPtr<ConstantBuffer<DirectX::XMMATRIX>> pPerObjectConstBuf = nullptr;
	};
}

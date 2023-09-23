#pragma once
#include "../../Core.h"
#include "D3D11.h"
#include "Transform.h"
#include "../../Input/IInputHandler.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Controller.h"
#include "../../Input/Mouse.h"
#include <memory>

namespace CCE::Graphics
{	
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	struct CCE_API Camera : protected Input::IInputHandler
	{
	public:
		Camera();
		~Camera();

		Transform transform;

		void SetProjection(float _near, float _far, float _fovV, float _fovH) noexcept;

		//const DirectX::XMMATRIX GetViewProjectionMatrix() const noexcept
		//{ return DirectX::XMMatrixMultiply(modelMatrix, viewProjectionMatrix); }
		
		const float GetNearPlane() const noexcept;
		const float GetFarPlane() const noexcept;
		const float GetVerticalFOV() const noexcept;
		const float GetHorizontalFOV() const noexcept;

		void CreateConstBufs();
		void SetFovAndLookDir() noexcept;
		void Update();

	private:
		float nearPlane = 0.1f;
		float farPlane = 20.0f;
		float fovVertical = 120.0f;
		float fovHorizontal = 160.0f;

		DirectX::XMVECTOR lookDir = {0, 0, 1};
		float camMovementDelta = 0.005f;

		struct CameraConstantBufs 
		{
			DirectX::XMMATRIX viewMatrix = { };
			DirectX::XMMATRIX projectionMatrix = { };

		} cameraConstBufs = { };

		std::shared_ptr<VSConstantBuffer<CameraConstantBufs>> pCameraConstBuf = nullptr;

		// Inherited via IInputHandler
		void InputCallback(const Input::Mouse* mouse,
			const Input::Keyboard* keyboard,
			const Input::Controller* controller) override;

	};
}

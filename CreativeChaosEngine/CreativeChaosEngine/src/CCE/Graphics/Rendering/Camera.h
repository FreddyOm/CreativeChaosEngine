#pragma once
#include <memory>
#include "D3D11.h"
#include "../../Core.h"
#include "..\..\ECS\Components\Transform.h"
#include "../../Input/Mouse.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Controller.h"
#include "../../Input/IInputHandler.h"

namespace CCE::Graphics
{	
	template<typename C>
	class ConstantBuffer;
	template<typename C>
	class VSConstantBuffer;

	struct CCE_API Camera : protected Input::IInputHandler
	{
	private:
		enum class ProjectionType : unsigned char
		{
			PERSPECTIVE = 0,
			ORTHOGRAPHIC = 1,
		};

	public:
		Camera();
		~Camera();

		ECS::Components::Transform transform;

		void SetProjectionData(float _near, float _far, float _fovV) noexcept;
		void SetProjectionType(unsigned char type) noexcept;
		
		const float GetNearPlane() const noexcept;
		const float GetFarPlane() const noexcept;
		const float GetVerticalFOV() const noexcept;

		void CreateConstBufs();
		void SetFovAndLookDir() noexcept;
		void Update();

		float fovVertical = 30.0f;

	private:
		float nearPlane = 0.1f;
		float farPlane = 50.0f;

		DirectX::XMFLOAT3 lookDir = {0, 0, 1};
		float camMovementDelta = 0.003f;
		float defaultCamMovementDelta = 0.003f;
		float fastCamMovementDelta = 0.007f;
		float camPanDelta = 0.05f;
		float camZoomDelta = 0.5f;
		float camRotXDelta = 0.10f;
		float camRotYDelta = 0.10f;

		ProjectionType projType = ProjectionType::PERSPECTIVE;

		struct CameraConstantBufs 
		{
			alignas(16) DirectX::XMFLOAT4X4 viewMatrix = { };
			alignas(16) DirectX::XMFLOAT4X4 projectionMatrix = { };

		} cameraConstBufs = { };

		std::shared_ptr<VSConstantBuffer<CameraConstantBufs>> pCameraConstBuf = nullptr;

		// Inherited via IInputHandler
		void InputCallback(const Input::Mouse* mouse,
			const Input::Keyboard* keyboard,
			const Input::Controller* controller) override;
	};
}

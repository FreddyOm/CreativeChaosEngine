#include "Camera.h"
#include "../Rendering/../../Manager/ProfilingManager.h"
#include "../RenderPipeline.h"
#include "Bindable\ConstantBuffer.h"
#include "../../Manager/InputManager.h"
#include "../../Analysis/Time.h"

namespace CCE::Graphics
{
	Camera::Camera()
	{
		transform.SetTranslation({0.0f, 0.0f, -2.0f});
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
		SetFovAndLookDir();
		pCameraConstBuf = std::make_shared<VSConstantBuffer<CameraConstantBufs>>(cameraConstBufs, 1);
	}

	// TODO: Only do this when necessary!
	void Camera::SetFovAndLookDir() noexcept
	{
		using namespace DirectX;

		ZeroMemory(&cameraConstBufs.viewMatrix, sizeof(DirectX::XMMATRIX));
		ZeroMemory(&cameraConstBufs.projectionMatrix, sizeof(DirectX::XMMATRIX));

		float fovRadians = (fovVertical / 360.0f) * XM_2PI;

		cameraConstBufs.projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians,
			static_cast<float>(RenderPipeline::Instance->GetRenderTargetWidth()) /
			static_cast<float>(RenderPipeline::Instance->GetRenderTargetHeight()),
			0.1f,
			100.0f);

		 cameraConstBufs.viewMatrix = XMMatrixLookAtLH(
			{ transform.Position().x, transform.Position().y, transform.Position().z },
			 { transform.Position().x, transform.Position().y, transform.Position().z + 1 }, {0,1,0,0});
	}

	void Camera::Update()
	{
		PollInput();

		SetFovAndLookDir();

		HRESULT hr;

		pCameraConstBuf->UpdateConstantBuffer(cameraConstBufs);
		pCameraConstBuf->Bind();
	}

	void Camera::PollInput()
	{
		using namespace CCE::Input;
		using CCE::Input::InputDevice;

		if (InputManager::Instance->mouse.rightMouseButton == InputDevice::ButtonState::PRESSED)
		{
			#pragma region wasd

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_W] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y, transform.Position().z + (camMovementDelta * (float)CCE::Time::deltaTime) });
			}

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_A] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x - (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().y, transform.Position().z });
			}

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_S] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y, transform.Position().z - (camMovementDelta * (float)CCE::Time::deltaTime) });
			}

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_D] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x + (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().y, transform.Position().z });
			}

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_Q] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y + (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().z });
			}

			if (InputManager::Instance->keyboard.keys[(int)InputDevice::Keycode::KEY_E] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y - (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().z });
			}

#pragma endregion wasd
		}
		else if (InputManager::Instance->mouse.middleMouseButton == InputDevice::ButtonState::PRESSED)
		{
			transform.SetTranslation({ transform.Position().x - InputManager::Instance->mouse.deltaX * (camMovementDelta * (float)CCE::Time::deltaTime),
				transform.Position().y + InputManager::Instance->mouse.deltaY * (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().z });
		}
	}
}

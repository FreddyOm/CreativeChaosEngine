#include "Camera.h"
#include "../RenderPipeline.h"
#include "../../Analysis/Time.h"
#include "Bindable/ConstantBuffer.h"
#include "../../Manager/InputManager.h"
#include "../Rendering/../../Manager/ProfilingManager.h"

namespace CCE::Graphics
{
	Camera::Camera()
	{
		transform.SetTranslation({0.0f, 0.0f, -10.0f});
		SetFovAndLookDir();
		CreateConstBufs();

		InputManager::Instance->RegisterInputCallback(*this);

		REGISTER_LEAK_DETECT;
	}

	Camera::~Camera()
	{
		UNREGISTER_LEAK_DETECT;
	}

#pragma region setter

	void Camera::SetProjectionData(float _near, float _far, float _fovV) noexcept
	{
		nearPlane = _near;
		farPlane = _far;
		fovVertical = _fovV;
	}

	void Camera::SetProjectionType(unsigned char type) noexcept
	{
		projType = (ProjectionType)type;
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

		float width = static_cast<float>(RenderPipeline::Instance->GetRenderTargetWidth());
		float height = static_cast<float>(RenderPipeline::Instance->GetRenderTargetHeight());

		XMMATRIX fovMatr = XMMatrixIdentity();
		if (projType == ProjectionType::PERSPECTIVE)
		{
			fovMatr = XMMatrixPerspectiveFovLH(XMConvertToRadians(fovVertical), (float)width / (float)height, nearPlane, farPlane);
		}
		else 
		{
			fovMatr = XMMatrixOrthographicLH((float)width, (float)height, nearPlane, farPlane);
		}
		
		XMMATRIX viewMatr = XMMatrixLookAtLH(
			{ transform.Position().x, transform.Position().y, transform.Position().z },
			{ transform.Position().x, transform.Position().y, transform.Position().z + 1 }, { 0,1,0,0 });

		XMStoreFloat4x4(&cameraConstBufs.projectionMatrix, fovMatr);
		XMStoreFloat4x4(&cameraConstBufs.viewMatrix, viewMatr);
	}

	void Camera::Update()
	{
		SetFovAndLookDir();

		pCameraConstBuf->UpdateConstantBuffer(cameraConstBufs);
		pCameraConstBuf->Bind();
	}

	// TODO: Only do this on the viewport cam (create another class for the viewport cam or derive from it -> indirection[!])
	// Also, keep in mind virtual functions are runtime performance critical!
	void Camera::InputCallback(const Input::Mouse* mouse, const Input::Keyboard* keyboard, const Input::Controller* controller)
	{
		using namespace CCE::Input;
		using CCE::Input::InputDevice;
		if (mouse->middleMouseButton == InputDevice::ButtonState::PRESSED)
		{
			transform.SetTranslation({ transform.Position().x - mouse->deltaX * (camPanDelta * (float)CCE::Time::deltaTime),
				transform.Position().y + mouse->deltaY * (camPanDelta * (float)CCE::Time::deltaTime), transform.Position().z });
		}
		else if (mouse->rightMouseButton == InputDevice::ButtonState::PRESSED)
		{
			camMovementDelta = keyboard->keys[(int)InputDevice::Keycode::SHIFT] == InputDevice::ButtonState::PRESSED ? fastCamMovementDelta : defaultCamMovementDelta;

			#pragma region wasd

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_W] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y, transform.Position().z + (camMovementDelta * (float)CCE::Time::deltaTime) });
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_A] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x - (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().y, transform.Position().z });
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_S] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y, transform.Position().z - (camMovementDelta * (float)CCE::Time::deltaTime) });
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_D] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x + (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().y, transform.Position().z });
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_Q] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y + (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().z });
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_E] == InputDevice::ButtonState::PRESSED)
			{
				transform.SetTranslation({ transform.Position().x, transform.Position().y - (camMovementDelta * (float)CCE::Time::deltaTime), transform.Position().z });
			}

#pragma endregion wasd

			#pragma region rotate cam



			#pragma endregion rotate cam

			return;
		}
		
		#pragma region zoom

		if (mouse->wheelDelta != 0)
		{
			transform.SetTranslation({ transform.Position().x, transform.Position().y ,transform.Position().z + mouse->wheelDelta * camZoomDelta * (float)CCE::Time::deltaTime });
		}

		#pragma endregion zoom

	}
}

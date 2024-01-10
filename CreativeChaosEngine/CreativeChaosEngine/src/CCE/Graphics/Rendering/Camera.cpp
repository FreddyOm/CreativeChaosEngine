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
		
		
		auto forward = transform.Forward();
		auto focusPoint = XMVectorAdd(XMLoadFloat3(&transform.Position()), forward);
		XMMATRIX viewMatr = XMMatrixLookAtLH(XMLoadFloat3(&transform.Position()),
			focusPoint, {0, 1, 0});
		

		XMStoreFloat4x4(&cameraConstBufs.projectionMatrix, fovMatr);
		XMStoreFloat4x4(&cameraConstBufs.viewMatrix, viewMatr);
	}

	void Camera::Update()
	{
		SetFovAndLookDir();

		pCameraConstBuf->UpdateConstantBuffer(cameraConstBufs);
		pCameraConstBuf->DynamicBind();
	}

	// TODO: Only do this on the viewport cam (create another class for the viewport cam or derive from it -> indirection[!])
	// Also, keep in mind virtual functions are runtime performance critical!
	void Camera::InputCallback(const Input::Mouse* mouse, const Input::Keyboard* keyboard, const Input::Controller* controller)
	{
		using namespace CCE::Input;
		using CCE::Input::InputDevice;
		if (mouse->middleMouseButton == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3( &transform.Position(), 
				XMVectorAdd( XMLoadFloat3( &transform.Position() ), 
				( transform.Right() * static_cast<float>( -mouse->deltaX ) * camPanDelta * static_cast<float>(CCE::Time::deltaTime) ) ) );


			XMStoreFloat3( &transform.Position(), 
				XMVectorAdd( XMLoadFloat3( &transform.Position() ), 
				( transform.Up() * static_cast<float>(mouse->deltaY ) * camPanDelta * static_cast<float>(CCE::Time::deltaTime) ) ) );
		
			return;
		}
		else if (mouse->rightMouseButton == InputDevice::ButtonState::PRESSED)
		{
			camMovementDelta = keyboard->keys[(int)InputDevice::Keycode::SHIFT] == InputDevice::ButtonState::PRESSED ? fastCamMovementDelta : defaultCamMovementDelta;

			#pragma region wasd

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_W] == InputDevice::ButtonState::PRESSED)
			{
				XMVECTOR deltaPos = XMVectorAdd(XMLoadFloat3(&transform.Position()), (transform.Forward() * camMovementDelta * (float)CCE::Time::deltaTime));
				XMStoreFloat3(&transform.Position(), deltaPos);
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_A] == InputDevice::ButtonState::PRESSED)
			{
				XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (-transform.Right() * camMovementDelta * (float)CCE::Time::deltaTime)));
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_S] == InputDevice::ButtonState::PRESSED)
			{
				XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (-transform.Forward() * camMovementDelta * (float)CCE::Time::deltaTime)));
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_D] == InputDevice::ButtonState::PRESSED)
			{
				XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (transform.Right() * camMovementDelta * (float)CCE::Time::deltaTime)));
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_Q] == InputDevice::ButtonState::PRESSED)
			{
				XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (transform.Up() * camMovementDelta * (float)CCE::Time::deltaTime)));
			}

			if (keyboard->keys[(int)InputDevice::Keycode::KEY_E] == InputDevice::ButtonState::PRESSED)
			{
				XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (-transform.Up() * camMovementDelta * (float)CCE::Time::deltaTime)));
			}

#pragma endregion wasd

			#pragma region rotate cam

			float deltaX = mouse->deltaX * camRotYDelta * (float)CCE::Time::deltaTime;
			float deltaY = mouse->deltaY * camRotXDelta * (float)CCE::Time::deltaTime;
			// Make sure, the global up vector still holds true in any case!!
			if (transform.Rotation().x + deltaY > -80.0f && transform.Rotation().x + deltaY < 80.0f)
			{
				transform.SetRotation({ transform.Rotation().x + deltaY,
				transform.Rotation().y + deltaX, transform.Rotation().z });
			}

			#pragma endregion rotate cam

			return;
		}

		#pragma region zoom

		if (mouse->wheelDelta != 0)
		{
			XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (transform.Forward() * mouse->wheelDelta * camZoomDelta * (float)CCE::Time::deltaTime)));
		}

		#pragma endregion zoom

		#pragma region controller

		#pragma region pan

		if (controller->LNorth == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(),
				XMVectorAdd(XMLoadFloat3(&transform.Position()),
					(transform.Up() * 0.2f * camPanDelta * static_cast<float>(CCE::Time::deltaTime))));
		}
		if (controller->LEast == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(),
				XMVectorAdd(XMLoadFloat3(&transform.Position()),
					(transform.Right() * 0.2f * camPanDelta * static_cast<float>(CCE::Time::deltaTime))));
		}
		if (controller->LSouth == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(),
				XMVectorAdd(XMLoadFloat3(&transform.Position()),
					(-transform.Up() * 0.2f * camPanDelta * static_cast<float>(CCE::Time::deltaTime))));
		}
		if (controller->LWest == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(),
				XMVectorAdd(XMLoadFloat3(&transform.Position()),
					(-transform.Right() * 0.2f * camPanDelta * static_cast<float>(CCE::Time::deltaTime))));
		}
		
		#pragma endregion pan

		#pragma region move
		
		camMovementDelta = controller->RTrigger.value > 0.5f ? fastCamMovementDelta : defaultCamMovementDelta;
		
		if (controller->LJoypad.y.state == InputDevice::AxisState::AXIS_MOVED)
		{
			XMVECTOR deltaPos = XMVectorAdd(XMLoadFloat3(&transform.Position()), 
				(transform.Forward() * controller->LJoypad.y.value * camMovementDelta * (float)CCE::Time::deltaTime));
			XMStoreFloat3(&transform.Position(), deltaPos);
		}

		if (controller->LJoypad.x.state == InputDevice::AxisState::AXIS_MOVED)
		{
			XMStoreFloat3(&transform.Position(), 
				XMVectorAdd(XMLoadFloat3(&transform.Position()), 
					(transform.Right() * controller->LJoypad.x.value * camMovementDelta* (float)CCE::Time::deltaTime)));
		}

		if (controller->LShoulder == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (transform.Up() * camMovementDelta * (float)CCE::Time::deltaTime)));
		}

		if (controller->RShoulder == InputDevice::ButtonState::PRESSED)
		{
			XMStoreFloat3(&transform.Position(), XMVectorAdd(XMLoadFloat3(&transform.Position()), (-transform.Up() * camMovementDelta * (float)CCE::Time::deltaTime)));
		}

		#pragma endregion move

		#pragma region rotate cam

		float lookDeltaX = controller->RJoypad.x.value * camRotYDelta * (float)CCE::Time::deltaTime;
		float lookDeltaY = -controller->RJoypad.y.value * camRotXDelta * (float)CCE::Time::deltaTime;
		// Make sure, the global up vector still holds true in any case!!
		if (transform.Rotation().x + lookDeltaY > -80.0f && transform.Rotation().x + lookDeltaY < 80.0f)
		{
			transform.SetRotation({ transform.Rotation().x + lookDeltaY,
			transform.Rotation().y + lookDeltaX, transform.Rotation().z });
		}

		#pragma endregion rotate cam

		#pragma endregion controller
	}
}

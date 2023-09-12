#include "Camera.h"
#include "../Rendering/../../Manager/ProfilingManager.h"

namespace CCE::Graphics
{
	Camera::Camera()
	{
		using namespace DirectX;

		ZeroMemory(&projectionMatrix, sizeof(projectionMatrix));

		float h, w, Q;

		w = (float)1 / (float)tan(fovHorizontal * 0.5);
		h = (float)1 / (float)tan(fovVertical * 0.5);
		Q = farPlane / (farPlane - nearPlane);

		projectionMatrix =
		{
			w, 0, 0, 0,
			0, h, 0, 0,
			0, 0, Q, 1,
			0, 0, -Q * nearPlane, 0
		};

		transform.SetPosition({0.0f, 0.0f, -2.0f});

		viewMatrix = XMMatrixLookAtLH(
			{ transform.Position().x, transform.Position().y, transform.Position().z },
			g_XMZero, { 0,1,0,1 });
	
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

	void Camera::Update()
	{

	}
}

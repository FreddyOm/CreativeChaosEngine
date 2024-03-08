#include "Transform.h"
#include "../../Thirdparty/src/optick.h"

namespace CCE::ECS::Components
{
	Transform::Transform()
	{
		OPTICK_EVENT();
		SetTranslation(position);
		SetRotation(rotation);
		SetScale(scale);
	}

	DirectX::XMMATRIX Transform::GetTransformationMatrix() const
	{
		OPTICK_EVENT();
		return (DirectX::XMLoadFloat4x4(&rotationMatrix) *
			DirectX::XMLoadFloat4x4(&scaleMatrix)) *
			DirectX::XMLoadFloat4x4(&translationMatrix);
	}

	void Transform::SetTranslation(DirectX::XMFLOAT3 _position)
	{
		OPTICK_EVENT();
		position = _position;
		DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z));
	}

	void Transform::SetScale(DirectX::XMFLOAT3 _scale)
	{
		OPTICK_EVENT();
		scale = _scale;
		DirectX::XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
	}

	void Transform::SetRotation(DirectX::XMFLOAT3 _rotation)
	{
		OPTICK_EVENT();
		rotation = _rotation;
		DirectX::XMStoreFloat4x4(&rotationMatrix,
			DirectX::XMMatrixRotationRollPitchYaw(
				DirectX::XMConvertToRadians(rotation.x),
				DirectX::XMConvertToRadians(rotation.y),
				DirectX::XMConvertToRadians(rotation.z)));
	}

	/// <summary>
	/// Transforms the global forward vector into the current model space.
	/// </summary>
	/// <returns>The local Forward-Vector as XMVECTOR</returns>
	DirectX::XMVECTOR Transform::Forward() const
	{
		OPTICK_EVENT();
		using namespace DirectX;

		XMVECTOR gForw = XMVectorSet(0, 0, 1, 0);
		auto forw = XMVector3Transform(gForw, XMLoadFloat4x4(&rotationMatrix));
		return XMVector3Normalize(forw);
	}

	/// <summary>
		/// Transforms the global up vector into the current model space.
		/// </summary>
		/// <returns>The local Up-Vector as XMMVECTOR</returns>
	DirectX::XMVECTOR Transform::Up() const
	{
		OPTICK_EVENT();
		using namespace DirectX;

		XMVECTOR gUp = XMVectorSet(0, 1, 0, 0);
		auto right = XMVector3Transform(gUp, XMLoadFloat4x4(&rotationMatrix));

		return XMVector3Normalize(right);
	}

	/// <summary>
	/// Transforms the global right vector into the current model space.
	/// </summary>
	/// <returns>The local Right-Vector as XMMVECTOR</returns>
	DirectX::XMVECTOR Transform::Right() const
	{
		OPTICK_EVENT();
		using namespace DirectX;

		XMVECTOR gRight = XMVectorSet(1, 0, 0, 0);
		auto right = XMVector3Transform(gRight, XMLoadFloat4x4(&rotationMatrix));

		return XMVector3Normalize(right);
	}

	DirectX::XMFLOAT3& Transform::Position()		// @TODO: Make const& and const func
	{
		return position;
	}

	DirectX::XMFLOAT3& Transform::Scale()			// @TODO: Make const& and const func
	{
		return scale;
	}

	DirectX::XMFLOAT3& Transform::Rotation()		// @TODO: Make const& and const func
	{
		return rotation;
	}

	DirectX::XMFLOAT4X4 Transform::GetRotationMatrix() const
	{
		return rotationMatrix;
	}
}

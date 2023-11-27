#pragma once
#include "D3D11.h"
#include "../../Core.h"

struct CCE_API Transform
{
public:
	Transform()
	{
		SetTranslation(position);
		SetRotation(rotation);
		SetScale(scale);
	}

	DirectX::XMMATRIX GetTransformationMatrix() const
	{
		return (DirectX::XMLoadFloat4x4(&rotationMatrix) * 
			DirectX::XMLoadFloat4x4(&scaleMatrix)) *
			DirectX::XMLoadFloat4x4(&translationMatrix);
	}

	void SetTranslation(DirectX::XMFLOAT3 _position)
	{
		position = _position;
		DirectX::XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z));
	}

	void SetScale(DirectX::XMFLOAT3 _scale)
	{
		scale = _scale;
		DirectX::XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScaling(scale.x, scale.y, scale.z));
	}

	void SetRotation(DirectX::XMFLOAT3 _rotation)
	{
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
	DirectX::XMVECTOR Forward()
	{
		using namespace DirectX;
		
		XMVECTOR gForw = XMVectorSet(0, 0, 1, 0);
		auto forw = XMVector3Transform(gForw, XMLoadFloat4x4(&rotationMatrix));
		return XMVector3Normalize(forw);
	}

	/// <summary>
	/// Transforms the global up vector into the current model space.
	/// </summary>
	/// <returns>The local Up-Vector as XMMVECTOR</returns>
	DirectX::XMVECTOR Up()
	{
		using namespace DirectX;

		XMVECTOR gUp = XMVectorSet(0, 1, 0, 0);
		auto right = XMVector3Transform(gUp, XMLoadFloat4x4(&rotationMatrix));

		return XMVector3Normalize(right);
	}

	/// <summary>
	/// Transforms the global right vector into the current model space.
	/// </summary>
	/// <returns>The local Right-Vector as XMMVECTOR</returns>
	DirectX::XMVECTOR Right()
	{
		using namespace DirectX;

		XMVECTOR gRight = XMVectorSet(1, 0, 0, 0);
		auto right = XMVector3Transform(gRight, XMLoadFloat4x4(&rotationMatrix));

		return XMVector3Normalize(right);
	}

	DirectX::XMFLOAT3& Position()
	{
		return position;
	}

	DirectX::XMFLOAT3& Scale()
	{
		return scale;
	}

	DirectX::XMFLOAT3& Rotation()
	{
		return rotation;
	}

	DirectX::XMFLOAT4X4 GetRotationMatrix()
	{
		return rotationMatrix;
	}

protected:
	DirectX::XMFLOAT3 position = {0,0,0};
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };

private:
	DirectX::XMFLOAT4X4 rotationMatrix = {};
	DirectX::XMFLOAT4X4 scaleMatrix = {};
	DirectX::XMFLOAT4X4 translationMatrix = {};
};

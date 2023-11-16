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

	DirectX::XMFLOAT3 Position()
	{
		return position;
	}

	DirectX::XMFLOAT3 Scale()
	{
		return scale;
	}

	DirectX::XMFLOAT3 Rotation()
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

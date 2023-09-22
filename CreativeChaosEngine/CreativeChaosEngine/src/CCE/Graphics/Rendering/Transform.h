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
		//return translationMatrix;
		return translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;
	}

	void SetTranslation(DirectX::XMFLOAT3 _position)
	{
		position = _position;
		translationMatrix = DirectX::XMMatrixTranslation(_position.x, _position.y, _position.z);
	}

	void SetScale(DirectX::XMFLOAT3 _scale)
	{
		scale = _scale;
		scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	}

	void SetRotation(DirectX::XMFLOAT3 _rotation)
	{
		rotation = _rotation;
		rotationXMatrix = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.x));
		rotationYMatrix = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));
		rotationZMatrix = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.z));
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

protected:
	DirectX::XMFLOAT3 position = {0,0,0};
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 1,1,1 };

private:
	DirectX::XMMATRIX rotationXMatrix = {};
	DirectX::XMMATRIX rotationYMatrix = {};
	DirectX::XMMATRIX rotationZMatrix = {};
	DirectX::XMMATRIX scaleMatrix = {};
	DirectX::XMMATRIX translationMatrix = {};
};
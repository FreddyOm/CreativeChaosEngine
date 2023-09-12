#pragma once
#include "D3D11.h"
#include "../../Core.h"

struct CCE_API Transform
{
public:
	DirectX::XMMATRIX GetTransformationMatrix() const
	{
		return DirectX::XMMatrixMultiply(positionMatrix, DirectX::XMMatrixMultiply(scaleMatrix, rotationMatrix));;
	}

	void SetPosition(DirectX::XMFLOAT3 _position)
	{
		position = _position;
		positionMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	}

	void SetScale(DirectX::XMFLOAT3 _scale)
	{
		scale = _scale;
		scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	}

	void SetRotation(DirectX::XMFLOAT3 _rotation)
	{
		rotation = _rotation;
		rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
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
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

private:
	DirectX::XMMATRIX rotationMatrix;
	DirectX::XMMATRIX scaleMatrix;
	DirectX::XMMATRIX positionMatrix;
};
#pragma once
#include "../../graphics/rendering/D3D11/D3D11.h"
#include "../../core.h"

namespace CCE::ECS::Components
{
	class Transform
	{
		friend class Rigidbody;
	public:
		Transform();
		~Transform() = default;

		Transform(const Transform& other)
		{
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
		}

		Transform(Transform&& other) noexcept
		{
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
		}

		Transform& operator=(const Transform& other)
		{
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
			return *this;
		}

		Transform& operator=(Transform&& other) noexcept
		{
			position = other.position;
			rotation = other.rotation;
			scale = other.scale;
			return *this;
		}

		DirectX::XMMATRIX GetTransformationMatrix() const;

		DirectX::XMVECTOR Forward() const;
		DirectX::XMVECTOR Up() const;
		DirectX::XMVECTOR Right() const;

		DirectX::XMFLOAT3& Position();
		DirectX::XMFLOAT3& Scale();
		DirectX::XMFLOAT3& Rotation();

		void SetTranslation(DirectX::XMFLOAT3 _position);
		void SetScale(DirectX::XMFLOAT3 _scale);
		void SetRotation(DirectX::XMFLOAT3 _rotation);
	
		DirectX::XMFLOAT4X4 GetRotationMatrix() const;

	protected:
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 rotation = { 0,0,0 };
		DirectX::XMFLOAT3 scale = { 1,1,1 };

	private:
		DirectX::XMFLOAT4X4 rotationMatrix = {};
		DirectX::XMFLOAT4X4 scaleMatrix = {};
		DirectX::XMFLOAT4X4 translationMatrix = {};
	};
}

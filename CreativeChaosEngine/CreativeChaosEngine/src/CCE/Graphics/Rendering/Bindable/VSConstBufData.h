#pragma once
#include "../D3D11.h"

namespace CCE::Graphics
{
	struct VSConstBufData
	{
		VSConstBufData(const DirectX::XMMATRIX matrix, const DirectX::XMFLOAT4 color)
		{
			using namespace DirectX;
			XMStoreFloat4x4(&modelMatrix, matrix);
			baseColor = color;
		}

		VSConstBufData(const DirectX::XMFLOAT4X4 matrix, const DirectX::XMFLOAT4 color)
		{
			using namespace DirectX;
			modelMatrix = matrix;
			baseColor = color;
		}

		VSConstBufData(VSConstBufData&& other) noexcept
		{
			modelMatrix = other.modelMatrix;
			baseColor = other.baseColor;
		}

		alignas(16) DirectX::XMFLOAT4X4 modelMatrix{};
		alignas(32) DirectX::XMFLOAT4 baseColor{};
	};
}

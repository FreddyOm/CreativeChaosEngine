#pragma once
#include "../../Core.h"
#include "D3D11.h"
#include <DirectXMath.h>

namespace CCE::Graphics 
{
	using namespace DirectX;
	// TODO: Setup as proposed in RTR
	struct CCE_API Vertex
	{
		XMFLOAT3 Position;
		//XMFLOAT3 Color;
	};
}

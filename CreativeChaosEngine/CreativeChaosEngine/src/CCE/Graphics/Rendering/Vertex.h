#pragma once
#include "../../Core.h"
#include <DirectXMath.h>

namespace CCE::Graphics 
{
	using namespace DirectX;
	// TODO: Setup as proposed in RTR
	struct CCE_API Vertex
	{
		XMFLOAT3 Position;
		XMFLOAT2 UVCoord;
	};
}

#pragma once
#include "../../Core.h"
#include "D3D11.h"

namespace CCE::Graphics
{
	struct CCE_API IDrawable
	{
	public:
		virtual void DrawIndexed(UINT count) = 0;
	};
}

#pragma once
#include "../../Core.h"

namespace CCE::Graphics
{
	struct CCE_API IDrawable
	{
	public:
		virtual void DrawIndexed(UINT count) = 0;
	};
}

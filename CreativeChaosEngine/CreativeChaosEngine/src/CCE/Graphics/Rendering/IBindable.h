#pragma once
#include "../../Core.h"
#include "D3D11.h"

namespace CCE::Graphics
{
	struct CCE_API IBindable
	{
	public:
		virtual void Bind() = 0;
	};
}

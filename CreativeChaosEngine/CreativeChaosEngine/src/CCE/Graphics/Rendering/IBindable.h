#pragma once
#include "../../Core.h"

namespace CCE::Graphics
{
	struct CCE_API IBindable
	{
	public:
		virtual void Bind() = 0;
	};
}

#pragma once
#include "../Core.h"

namespace CCE::ECS
{
	struct IComponentBuffer
	{
	public:
		IComponentBuffer() {}
		virtual ~IComponentBuffer() {}
		virtual void EntityDestroyed(UINT64 entity) {}
	};
}

#pragma once
#include "../Core.h"
#include "Entity.h"

namespace CCE::Resources
{
	struct IComponentBuffer
	{
	public:
		IComponentBuffer() {}
		virtual ~IComponentBuffer() {}
		virtual void EntityDestroyed(Entity entity) {}
	};
}

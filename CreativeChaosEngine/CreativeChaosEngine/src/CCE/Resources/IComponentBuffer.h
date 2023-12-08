#pragma once
#include "../Core.h"
#include "Entity.h"

namespace CCE::Resources
{
	struct CCE_API IComponentBuffer
	{
	public:
		IComponentBuffer() {}
		virtual ~IComponentBuffer() = default;
		virtual void EntityDestroyed(Entity entity) {};

	};
}

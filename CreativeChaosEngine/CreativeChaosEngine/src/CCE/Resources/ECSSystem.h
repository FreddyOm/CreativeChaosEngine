#pragma once
#include "Entity.h"
#include <set>

namespace CCE::Resources
{
	class ECSSystem
	{
	public:
		std::set<Entity> mEntities;
	};
}
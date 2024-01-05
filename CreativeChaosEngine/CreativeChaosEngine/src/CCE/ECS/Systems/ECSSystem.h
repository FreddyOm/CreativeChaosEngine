#pragma once
#include "../Entity.h"
#include <set>

namespace CCE::ECS::Systems
{
	class ECSSystem
	{
	public:
		/// <summary>
		/// Each system has its own copy of all entites present in the system.
		/// Since an entity is an 8 byte integer, it doesn't matter if we safe
		/// pointers or directly the entity itself.
		/// </summary>
		std::set<CCE::ECS::Entity> mEntities;
	};
}
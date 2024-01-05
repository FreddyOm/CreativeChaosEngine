#pragma once
#include <set>
#include <vector>
#include "../Resources/Entity.h"
#include "../Manager/ProfilingManager.h"

namespace CCE::Scene
{
	class Scene
	{
	public:
		Scene()
		{
			REGISTER_LEAK_DETECT;
		}

		~Scene()
		{
			UNREGISTER_LEAK_DETECT;
		}

		void UpdateScene();
		CCE::Resources::Entity& AddEntity();
		void RemoveEntity(CCE::Resources::Entity& entity);
		
		/// <summary>
		/// Finds an entity in the scene by it's type. The first encounter is returned.
		/// </summary>
		/// <typeparam name="">The component type to look for in the entities.</typeparam>
		/// <returns>A reference to the found entity.</returns>
		template<typename T>
		CCE::Resources::Entity& FindEntityOfType()
		{
			for (auto& entity : entities)
			{
				if(CCE::Resources::EntityComponentSystem::Instance->HasEntityComponent<T>(entity))
				{
					return entity;
				}
			}
		}

		/// <summary>
		/// Finds all entities in the scene by their type. All encounters are returned.
		/// </summary>
		/// <typeparam name="">The component type to look for in the entities.</typeparam>
		/// <returns>A list of one or more found entities.</returns>
		template<typename T>
		std::vector<CCE::Resources::Entity> FindEntitiesOfType()
		{
			std::vector<CCE::Resources::Entity> _entities;

			for (auto& entity : entities)
			{
				if (CCE::Resources::EntityComponentSystem::Instance->HasEntityComponent<T>(entity))
				{
					_entities.push_back(entity);
				}
			}

			return _entities;
		}

	private:
		/// <summary>
		/// The entities associated with this scene. Don't iterate over them,
		/// use the ecs buffers for most of the work. Only use this when querying
		/// entities in this scene specifically!
		/// </summary>
		std::set<CCE::Resources::Entity> entities;
	};
}

#pragma once
#include <set>
#include <vector>
#include "../Resources/Entity.h"
#include "../Manager/ProfilingManager.h"

namespace CCE::Scene
{
	class Scene
	{
	private:
		std::set<CCE::Resources::Entity> entities;

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

		}

		/// <summary>
		/// Finds all entities in the scene by their type. All encounters are returned.
		/// </summary>
		/// <typeparam name="">The component type to look for in the entities.</typeparam>
		/// <returns>A list of one or more found entities.</returns>
		template<typename T>
		std::vector<CCE::Resources::Entity> FindEntitiesOfType()
		{

		}
	};
}

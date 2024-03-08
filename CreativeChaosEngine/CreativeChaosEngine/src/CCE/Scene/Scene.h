#pragma once
#include <set>
#include <vector>
#include "../Manager/ProfilingManager.h"
#include "../ECS/EntityComponentSystem.h"
#include "../ECS/Entity.h"
#include "../Manager/InputManager.h"

namespace CCE::Scene
{
	class Scene : protected Input::IInputHandler
	{
	public:
		Scene()
		{
			REGISTER_LEAK_DETECT;
			REGISTER_INPUT_CALLBACK;
		}

		~Scene()
		{
			UNREGISTER_LEAK_DETECT;
		}

		void SetupScene();
		void UpdateScene();
		void ResetScene();

		ECS::Entity& AddEntity();
		void RemoveEntity(ECS::Entity& entity);
		
		/// <summary>
		/// Finds an entity in the scene by it's type. The first encounter is returned.
		/// </summary>
		/// <typeparam name="">The component type to look for in the entities.</typeparam>
		/// <returns>A reference to the found entity.</returns>
		template<typename T>
		ECS::Entity& FindEntityOfType() const
		{
			for (auto& entity : entities) // @TODO: This is slow because ít is a set!!
			{
				if(ECS::EntityComponentSystem::Instance->HasEntityComponent<T>(entity))
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
		std::vector<ECS::Entity> FindEntitiesOfType() const
		{
			std::vector<ECS::Entity> _entities;

			for (auto& entity : entities) // @TODO: This is slow because ít is a set!!
			{
				if (ECS::EntityComponentSystem::Instance->HasEntityComponent<T>(entity))
				{
					_entities.push_back(entity);
				}
			}

			return _entities;
		}

	private:

		// Inherited via IInputHandler
		void InputCallback(const Input::Mouse* mouse,
			const Input::Keyboard* keyboard,
			const Input::Controller* controller) override;

	private:
		/// <summary>
		/// The entities associated with this scene. Don't iterate over them,
		/// use the ECS buffers for most of the work. Only use this when querying
		/// entities in this scene specifically!
		/// </summary>
		std::set<ECS::Entity> entities;
	};
}

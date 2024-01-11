#include "Scene.h"

namespace CCE::Scene
{
	/// <summary>
	/// Updates scene by referring to the ECS to update the registered components
	/// </summary>
	void Scene::UpdateScene()
	{
		// @TODO: Update all components (NOT THE ENTITIES!!)
		// Later this means updating the script behaviour for example

		// @TODO: Maybe I don't even need a Scene as its own class
		// The different systems aren't centralized and therefore also 
		// cannot really be aggregated together!
	}

	/// <summary>
	/// Adds a new entity to the scene by inserting it in the scene entity set.
	/// It can therefore be associatedwith this scene.
	/// </summary>
	/// <returns>A reference to the added entity.</returns>
	ECS::Entity& Scene::AddEntity()
	{
		using ECS = ECS::EntityComponentSystem;
		auto entity = ECS::Instance->CreateEntity();
		entities.insert(entity);
		return entity;
	}

	/// <summary>
	/// Removes the referenced entity from the scene and the whole ECS.
	/// This means all it's components are destroyed and given back to the ECS.
	/// </summary>
	/// <param name="entity">The entity to destroy.</param>
	void Scene::RemoveEntity(ECS::Entity& entity)
	{
		using ECS = ECS::EntityComponentSystem;
		ECS::Instance->DestroyEntity(entity);
		entities.erase(entity);
	}
}

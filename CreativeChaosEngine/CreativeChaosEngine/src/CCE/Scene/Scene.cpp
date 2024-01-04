#include "Scene.h"
#include "../Resources/EntityComponentSystem.h"

namespace CCE::Scene
{
	/// <summary>
	/// Updates scene by referring to the ECS to update the registered components
	/// </summary>
	void Scene::UpdateScene()
	{
		// TODO: Update all components (NOT THE ENTITIES!!)
	}

	/// <summary>
	/// Adds a new entity to the scene by inserting it in the scene entity set.
	/// It can therefore be associatedwith this scene.
	/// </summary>
	/// <returns>A reference to the added entity.</returns>
	CCE::Resources::Entity& Scene::AddEntity()
	{
		using namespace CCE::Resources;
		auto& entity = EntityComponentSystem::Instance->CreateEntity();
		entities.insert(entity);
		return entity;
	}

	/// <summary>
	/// Removes the referenced entity from the scene and the whole ECS.
	/// This means all it's components are destroyed and given back to the ECS.
	/// </summary>
	/// <param name="entity">The entity to destroy.</param>
	void Scene::RemoveEntity(CCE::Resources::Entity& entity)
	{
		using namespace CCE::Resources;
		EntityComponentSystem::Instance->DestroyEntity(entity);
		entities.erase(entity);
	}
}

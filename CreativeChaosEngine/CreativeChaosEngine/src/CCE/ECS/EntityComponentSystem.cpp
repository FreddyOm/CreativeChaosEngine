#include "EntityComponentSystem.h"
#include "Entity.h"
#include "IComponentBuffer.h"
#include "ComponentBuffer.h"

namespace CCE::ECS
{	
	/// <summary>
	/// General initialization for any manager.
	/// </summary>
	void EntityComponentSystem::StartUp()
	{
		DASSERT(Instance == nullptr, "ECS was instantiated more than once!");
		Instance = this;

		this->Initialize();

		BaseManager::Init();

		LOGC("ECS initialized!", COLOR_BLUE);
	}

	/// <summary>
	/// General deinitialization for any manager
	/// </summary>
	void EntityComponentSystem::ShutDown()
	{
		LOGC("Shutting down ECS...", COLOR_BLUE);
		BaseManager::Deinit();
		Instance = nullptr;
	}

	/// <summary>
	/// Custom initialization process for ECS.
	/// </summary>
	void EntityComponentSystem::Initialize()
	{
		// Create the maximum number of entities
		for (int i = 0; i < MAX_ENTITIES; ++i)
		{
			mEntityPool.push(i); // The id of an entity initially equals to its index
		}

		RegisterComponent<Components::Transform>();
		RegisterComponent<Components::Rigidbody>();
		RegisterComponent<Components::Behaviour>();
		RegisterComponent<Components::Mesh>();
		RegisterComponent<Components::Material>();
		RegisterComponent<Components::SphereCollider>();
		RegisterComponent<Components::BoxCollider>();
	}

	/// <summary>
	/// Custom deinitialization process for ECS.
	/// </summary>
	void EntityComponentSystem::Deinitialize()
	{
		// @TODO: Check if anything should be released or smth
	}

	/// <summary>
	/// Takes one of the entities and returns it as used.
	/// </summary>
	/// <returns>The fetched entity.</returns>
	Entity EntityComponentSystem::CreateEntity()
	{
		DASSERT(mEntityCount < MAX_ENTITIES, "Can't create any more instances!");

		Entity entity(mEntityPool.front());
		mEntityPool.pop();
		++mEntityCount;

		return entity;
	}

	/// <summary>
	/// Returns the entity back to the entity pool.
	/// </summary>
	/// <param name="entity">The entity to destroy.</param>
	void EntityComponentSystem::DestroyEntity(Entity entity)
	{
		DASSERT(entity.Id < MAX_ENTITIES, "Invalid entity.");

		// Reset entity composition
		mEntityComposition[entity.Id] = 0;
		mEntityPool.push(entity.Id);
		--mEntityCount;

		EntityDestroyed(entity);
	}

	/// <summary>
	/// Whenever an entity changes its components, make sure to remove 
	/// it from the system / add it to the system in question.
	/// </summary>
	/// <param name="entity">The entity to change the signature of.</param>
	/// <param name="signature">The new signature.</param>
	void EntityComponentSystem::EntitySignatureChanged(UINT64 entity, DWORD signature)
	{
		// Change signature in every system
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			// Add signature to system where its not already contained in
			if ((signature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			// Remove signature from system where it is still apparent
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

	/// <summary>
	/// Calls the EntityDestroyed() function on every type buffer.
	/// </summary>
	/// <param name="entity">The entity to destroy.</param>
	void EntityComponentSystem::EntityDestroyed(Entity entity) const
	{
		// Remove destroyed entity from respective list
		for (auto const& pair : mComponents)
		{
			auto const& component = pair.second;
			component->EntityDestroyed(entity.Id);
		}

		// Remove destroyed entity from system
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity.Id);
		}
	}

	/// <summary>
	/// Instance reference for this subsystem.
	/// </summary>
	EntityComponentSystem* EntityComponentSystem::Instance = nullptr;
}

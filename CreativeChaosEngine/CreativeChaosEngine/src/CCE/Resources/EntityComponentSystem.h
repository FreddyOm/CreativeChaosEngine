#pragma once
#include "./Components/ComponentHeaders.h"
#include "../Manager/BaseManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "ComponentBuffer.h"
#include <unordered_map>
#include <typeindex>
#include "../Core.h"
#include "Entity.h"
#include <vector>
#include <memory>
#include <array>
#include <queue>

#define MAX_ENTITIES 4096

namespace CCE::Resources
{
	class CCE_API EntityComponentSystem : public BaseManager
	{
		friend struct Entity;
	public:
		EntityComponentSystem() = default;
		~EntityComponentSystem() = default;

		/// <summary>
		/// General initialization for any manager.
		/// </summary>
		void StartUp() override
		{
			DASSERT(Instance == nullptr, "ECS was instantiated more than once!");
			Instance = this;

			Initialize();

			initialized = true;

			LOGC("ECS initialized!", COLOR_BLUE);
		}

		void ShutDown() override
		{
			Deinitialize();

			LOGC("Shutting down ECS...", COLOR_BLUE);
			initialized = false;
			Instance = nullptr;
		}

		/// <summary>
	    /// Takes one of the entities and returns it as used.
	    /// </summary>
	    /// <returns>The fetched entity.</returns>
		Entity CreateEntity()
		{
			DASSERT(mEntityCount < MAX_ENTITIES, "Can't create any more instances!");

			Entity entity = mEntityPool.front();
			mEntityPool.pop();
			++mEntityCount;

			return entity;
		}

		/// <summary>
	    /// Returns the entity back to the entity pool.
	    /// </summary>
	    /// <param name="entity">The entity to destroy.</param>
		void DestroyEntity(Entity entity)
		{
			DASSERT(entity.Id < MAX_ENTITIES, "Invalid entity.");

			// Reset entity composition
			mEntityComposition[entity.Id] = 0;
			mEntityPool.push(entity);
			--mEntityCount;
		}

		/// <summary>
	    /// Calls the EntityDestroyed() function on every type buffer.
	    /// </summary>
	    /// <param name="entity">The entity to destroy.</param>
		void EntityDestroyed(Entity entity)
		{
			// Remove destroyed entity from respective list
			for (auto const& pair : mComponents)
			{
				auto const& component = pair.second;
				component->EntityDestroyed(entity);
			}
		}

		/// <summary>
	    /// Registers a type as a component. This process adds the type to the LUT and inserts the data in the component.
	    /// </summary>
	    /// <typeparam name="T">The component type to register.</typeparam>
		template<typename T>
		void RegisterComponent()
		{
			std::type_index typeIndex(typeid(T));

			DASSERT(ComponentTypeLUT.find(typeIndex) == ComponentTypeLUT.end(),
				"Component type already registered.");

			// Add component to LUT
			ComponentTypeLUT.insert({ typeIndex, (DWORD)1 << mRegisteredComponentIndex });

			auto ptr = new ComponentBuffer<T>();

			++mRegisteredComponentIndex;
		}

	private:
		/// <summary>
		/// Custom initialization process for ECS.
		/// </summary>
		void Initialize()
		{
			// Create the maximum number of entities
			for (int i = 0; i < MAX_ENTITIES; ++i)
			{
				mEntityPool.push(Entity(i));
			}

			RegisterComponent<Transform>();
			RegisterComponent<Rigidbody>();
			//RegisterComponent<Mesh>();
			//RegisterComponent<MeshRenderer>();
			//RegisterComponent<Collider>();
			//RegisterComponent<Script>();
		}

		/// <summary>
		/// Custom deinitialization process for ECS.
		/// </summary>
		void Deinitialize()
		{
			// free the memory
			for (auto& kvp : mComponents)
			{
				delete kvp.second;
			}

			initialized = false;
			Instance = nullptr;
		}

		/// <summary>
		/// Get the component buffer from the ECS.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>A pointer reference to the component buffer.</returns>
		template<typename T>
		IComponentBuffer* GetComponentBuffer()
		{
			std::type_index typeIndex(typeid(T));
			DASSERT(ComponentTypeLUT.find(typeIndex) != ComponentTypeLUT.end(),
				"Component not registered.");
			ComponentBuffer<T>* iCompBuf = dynamic_cast<IComponentBuffer*>(mComponents[typeIndex]);
			//std::shared_ptr<ComponentBuffer<T>> ret = std::static_pointer_cast<ComponentBuffer<T>>(iCompBuf);
			return iCompBuf;
		}

	public:
		static EntityComponentSystem* Instance;

		unsigned int MAX_COMPONENTS_PER_OBJECT = 16;
		std::unordered_map<std::type_index, DWORD> ComponentTypeLUT = {};

	private:
		std::queue<Entity> mEntityPool = {};
		std::array<DWORD, MAX_ENTITIES> mEntityComposition = {};
		std::unordered_map<std::type_index, IComponentBuffer*> mComponents = {};
		unsigned int mEntityCount = 0;
		unsigned int mRegisteredComponentIndex = 0; // Used to dynamically create bitmasks for checking the types
	};
}

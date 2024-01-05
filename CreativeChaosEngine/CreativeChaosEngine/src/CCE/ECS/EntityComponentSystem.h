#pragma once
#include "./Components/ComponentHeaders.h"
#include "../Manager/BaseManager.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "ComponentBuffer.h"
#include <unordered_map>
#include "ECSSystem.h"
#include <typeindex>
#include "../Core.h"
#include "Entity.h"
#include <vector>
#include <memory>
#include <array>
#include <queue>

#define MAX_ENTITIES 4096

// Entity Component System inspired by the great Austin Morlan (https://austinmorlan.com/)
// But changed up to work in a OOP way (although this might change in the future due 
// to performance reasons!)

namespace CCE::ECS
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
		Entity& CreateEntity()
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
			
			// Remove destroyed entity from system
			for (auto const& pair : mSystems)
			{
				auto const& system = pair.second;

				system->mEntities.erase(entity);
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
			// Add components with key 'typename' to componentbuffer table
			std::shared_ptr<IComponentBuffer> ptr = 
				std::dynamic_pointer_cast<IComponentBuffer>(std::make_shared<ComponentBuffer<T>>()); // TODO: Check if static_pointer_cast works
			mComponents.insert({ typeIndex, ptr });

			++mRegisteredComponentIndex;
		}

		/// <summary>
		/// Checks if a given entity is composed of a specific component.
		/// </summary>
		/// <typeparam name="T">The component to check for.</typeparam>
		/// <param name="entity">The entity to check on.</param>
		/// <returns>True if the entity is composed of the given type, false if not.</returns>
		template<typename T>
		bool HasEntityComponent(Entity& entity) const
		{
			return mEntityComposition[entity.Id] & ComponentTypeLUT(typeid(T));
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
				mEntityPool.push(Entity(i)); // The id of an entity initially equals to its index
			}

			RegisterComponent<Components::Transform>();
			RegisterComponent<Components::Rigidbody>();
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
			initialized = false;
			Instance = nullptr;
		}

		/// <summary>
		/// Get the component buffer from the ECS.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>A smart pointer reference to the component buffer.</returns>
		template<typename T>
		std::shared_ptr<ComponentBuffer<T>> GetComponentBuffer()
		{
			std::type_index typeIndex(typeid(T));
			DASSERT(ComponentTypeLUT.find(typeIndex) != ComponentTypeLUT.end(),
				"Component not registered.");

			return std::static_pointer_cast<ComponentBuffer<T>>(mComponents[typeIndex]);
		}
		
		/// <summary>
		/// Registers a system in a hash map in order to be able to delete destroyed enemies.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			const char* typeName = typeid(T).name();

			DASSERT(mSystems.find(typeName) == mSystems.end(),
				"Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			mSystems.insert({ typeName, system });
			return system;
		}
		
		/// <summary>
		/// Whenever an entity changes its components, make sure to remove 
		/// it from the system / add it to the system in question.
		/// </summary>
		/// <param name="entity">The entity to change the signature of.</param>
		/// <param name="signature">The new signature.</param>
		void EntitySignatureChanged(Entity entity, DWORD signature)
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
		
	public:
		static EntityComponentSystem* Instance;

		unsigned int MAX_COMPONENTS_PER_OBJECT = 16;
		std::unordered_map<std::type_index, DWORD> ComponentTypeLUT = {};

	private:
		unsigned int mEntityCount = 0;
		unsigned int mRegisteredComponentIndex = 0; // Used to dynamically create bitmasks for checking the types

		std::queue<Entity> mEntityPool = {};
		std::array<DWORD, MAX_ENTITIES> mEntityComposition = {};
		std::unordered_map<std::type_index, std::shared_ptr<IComponentBuffer>> mComponents = {};

		// Map from system type string pointer to a signature
		std::unordered_map<const char*, DWORD> mSignatures{};
	
		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<ECSSystem>> mSystems{};
	};
}

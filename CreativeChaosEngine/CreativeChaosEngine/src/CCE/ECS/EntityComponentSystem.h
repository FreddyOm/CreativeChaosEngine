#pragma once
#include "./Components/ComponentHeaders.h"
#include "../Manager/BaseManager.h"
#include "ComponentBuffer.h"
#include "../Analysis/Logger.h"
#include "../Analysis/Debug.h"
#include "Systems/ECSSystem.h"
#include <unordered_map>
#include <typeindex>
#include "../Core.h"
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
	struct Entity;
	class CCE_API EntityComponentSystem : public BaseManager
	{
		friend struct Entity;
	public:
		EntityComponentSystem() = default;
		~EntityComponentSystem() = default;

		void StartUp();
		void ShutDown();
				
		void Initialize();
		void Deinitialize();

		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void EntitySignatureChanged(Entity& entity, DWORD signature);
		void EntityDestroyed(Entity entity) const;
		

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
				std::static_pointer_cast<IComponentBuffer>(std::make_shared<ComponentBuffer<T>>()); // @TODO: Check if static_pointer_cast works
			mComponents.insert({ typeIndex, ptr });

			++mRegisteredComponentIndex;
		}

		/// <summary>
		/// Registers a system in a hash map in order to be able to delete destroyed entites.
		/// </summary>
		/// <typeparam name="T">The type of the system.</typeparam>
		/// <returns>A shared pointer to the system.</returns>
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
		/// Checks if a given entity is composed of a specific component.
		/// </summary>
		/// <typeparam name="T">The component to check for.</typeparam>
		/// <param name="entity">The entity to check on.</param>
		/// <returns>True if the entity is composed of the given type, false if not.</returns>
		template<typename T>
		bool HasEntityComponent(UINT64 entityId) const
		{
			return mEntityComposition[entityId] & ComponentTypeLUT(typeid(T));
		}

	private:

		/// <summary>
		/// Get the component buffer from the ECS.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>A smart pointer reference to the component buffer.</returns>
		template<typename T>
		std::shared_ptr<ComponentBuffer<T>> GetComponentBuffer() const
		{
			std::type_index typeIndex(typeid(T));
			DASSERT(ComponentTypeLUT.find(typeIndex) != ComponentTypeLUT.end(),
				"Component not registered.");

			return std::static_pointer_cast<ComponentBuffer<T>>(mComponents.at(typeIndex));
		}
		
	public:
		static EntityComponentSystem* Instance;

		unsigned int MAX_COMPONENTS_PER_OBJECT = 16;
		std::unordered_map<std::type_index, DWORD> ComponentTypeLUT = {};

	private:
		unsigned int mEntityCount = 0;
		unsigned int mRegisteredComponentIndex = 0; // Used to dynamically create bitmasks for checking the types

		std::queue<UINT64> mEntityPool = {};
		std::array<DWORD, MAX_ENTITIES> mEntityComposition = {};
		std::unordered_map<std::type_index, std::shared_ptr<IComponentBuffer>> mComponents = {};

		// Map from system type string pointer to a signature
		std::unordered_map<const char*, DWORD> mSignatures{};
	
		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<Systems::ECSSystem>> mSystems{};
	};
}

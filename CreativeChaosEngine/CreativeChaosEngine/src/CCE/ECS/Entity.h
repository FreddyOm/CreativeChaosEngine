#pragma once
#include "../core.h"
#include "../string/String.h"
#include "entity-component-system.h"

namespace CCE::ECS
{
	struct Entity
	{
	public:
		Entity()
		: Id(0)
		{};

		Entity(UINT64 id)
			: Id(id)
		{}

		Entity(String name)
			: Id(name.sId)
		{}

		Entity(String&& name)
			: Id(name.sId)
		{}

		~Entity() {};

		Entity(const Entity& other)
		{
			Id = other.Id;
		}

		Entity(Entity&& other) noexcept
		{
			Id = other.Id;
			other.Id = 0;
		}

		Entity& operator=(const Entity& other)
		{
			Id = other.Id;
			return *this;
		}

		Entity& operator=(Entity&& other) noexcept
		{
			Id = other.Id;
			other.Id = 0;
			return *this;
		}

		bool operator==(const Entity& other) const
		{
			return Id == other.Id;
		}

		bool operator!=(const Entity& other) const
		{
			return Id != other.Id;
		}
		
		bool operator<(const Entity& other) const noexcept
		{
			return Id < other.Id;
		}
		
		bool operator>(const Entity& other) const noexcept
		{
			return Id > other.Id;
		}

		/// <summary>
		/// Adds a component to the entity.
		/// </summary>
		/// <typeparam name="T">The type of the component to add.</typeparam>
		template<typename T>
		T& AddComponent()
		{
			OPTICK_EVENT();
			using ECS = EntityComponentSystem;

			// @TODO: Somehow enable multi adding of one component type (e.g. multiple box colliders)
			DWORD component = ECS::Instance->mEntityComposition.at(Id);

			// Add component bit to this entity
			component |= ECS::Instance->ComponentTypeLUT[typeid(T)];

			// Notify the systems of the change
			ECS::Instance->EntitySignatureChanged(Id, component);

			// Add component to component list
			return ECS::Instance->GetComponentBuffer<T>()->InsertData(Id);
		}

		/// <summary>
		/// Removes the component data associated to this entity.
		/// </summary>
		/// <typeparam name="T">The type of component to remove.</typeparam>
		template<typename T>
		void RemoveComponent()
		{
			OPTICK_EVENT();
			using ECS = EntityComponentSystem;

			// @TODO: Check if there are multiple components of the same type
			UINT64 component = ECS::Instance->mEntityComposition.at(Id);

			// Clear the components bit from this entity
			component &= ~((UINT64)1 << ECS::Instance->ComponentTypeLUT[typeid(T)]);

			// Remove component from component list
			ECS::Instance->GetComponentBuffer()->RemoveData(*this);

			// Notify the systems of the change
			ECS::Instance->EntitySignatureChanged(*this, component);
		}

		/// <summary>
		/// Retrieves the component data associated with this entity.
		/// </summary>
		/// <typeparam name="T">The type of the component.</typeparam>
		/// <returns>A pointer to the component data.</returns>
		template<typename T>
		T* GetComponent() const
		{
			OPTICK_EVENT();
			
			using ECS = EntityComponentSystem;
			return ECS::Instance->GetComponentBuffer<T>()->GetData(Id);
		}

		/// <summary>
		/// Tries to get a component from an entity. Returns true if possible and false if not.
		/// </summary>
		/// <typeparam name="T">The type of the component to get.</typeparam>
		/// <param name="outRef">A reference to an empty component object.</param>
		/// <returns>True, if entity has this component, false if it doesnt.</returns>
		template<typename T>
		bool TryGetComponent(T& outRef) const
		{
			OPTICK_EVENT();
			using ECS = EntityComponentSystem;

			T* component = ECS::Instance->GetComponentBuffer<T>()->GetData(Id);

			if (nullptr != component)
			{
				outRef = *component;
				return true;
			}

			return false;
		}

	public:
		UINT64 Id = 0;

	};
}

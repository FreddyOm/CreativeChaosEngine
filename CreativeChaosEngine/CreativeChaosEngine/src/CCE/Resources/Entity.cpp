#include "Entity.h"
#include "EntityComponentSystem.h"
#include "ComponentBuffer.h"
#include <functional>

namespace CCE::Resources
{
	/// <summary>
	/// Adds a component to the entity.
	/// </summary>
	/// <typeparam name="T">The type of the component to add.</typeparam>
	template<typename T>
	void Entity::AddComponent()
	{
		using ECS = EntityComponentSystem;

		// TODO: Somehow enable multi adding of one component type (e.g. multiple box colliders)
		UINT64 component = ECS::Instance->mEntityComposition.at(Id);

		// Add component bit to this entity
		component |= ECS::Instance->ComponentTypeLUT[typeid(Transform)];

		// TODO: Add component to component list
		//ECS::Instance->GetComponentBuffer().InsertData(*this);
		ECS::Instance->GetComponentBuffer()->InsertData(*this, Transform());
	}

	/// <summary>
	/// Removes the component data associated to this entity.
	/// </summary>
	/// <typeparam name="T">The type of component to remove.</typeparam>
	template<typename T>
	void Entity::RemoveComponent()
	{
		using ECS = EntityComponentSystem;

		// TODO: Check if there are multiple components of the same type
		UINT64 component = ECS::Instance->mEntityComposition.at(Id);

		// Clear the components bit from this entity
		component &= ~((UINT64)1 << ECS::Instance->ComponentTypeLUT[typeid(Transform)]);

		// TODO: Remove component from component list
		ECS::Instance->GetComponentBuffer()->RemoveData(*this);
	}

	/// <summary>
	/// Retrieves the component data associated with this entity.
	/// </summary>
	/// <typeparam name="T">The type of the component.</typeparam>
	/// <returns>A reference to the component data.</returns>
	template<typename T>
	T& Entity::GetComponent()
	{
		using ECS = EntityComponentSystem;
		ECS::Instance->GetComponentBuffer()->GetData(*this);
	}

	/// <summary>
	/// Tries to get a component from an entity. Returns true if possible and false if not.
	/// </summary>
	/// <typeparam name="T">The type of the component to get.</typeparam>
	/// <param name="outRef">A reference to an empty component object.</param>
	/// <returns>True, if entity has this component, false if it doesnt.</returns>
	template<typename T>
	bool Entity::TryGetComponent(T& outRef)
	{
		using ECS = EntityComponentSystem;
		//T& component = ECS::Instance->GetComponentBuffer<T>().GetData(*this);
		//outRef = component;
		return false;
	}
	
	/// <summary>
	/// Operator overloading for use in unordered map.
	/// </summary>
	/// <param name="lhs">The first entity.</param>
	/// <param name="rhs">The other entity to compare to.</param>
	/// <returns>True if this entity ID is equal to the others ID.</returns>
	bool operator==(const Entity& lhs, const Entity& rhs) noexcept {
		return rhs.Id == lhs.Id;
	}

	/// <summary>
	/// Operator overloading for use in unordered map.
	/// </summary>
	/// <param name="lhs">The first entity.</param>
	/// <param name="rhs">The other entity to compare to.</param>
	/// <returns>True if this entity ID is greater than the others ID.</returns>
	bool operator<(const Entity& lhs, const Entity& rhs) noexcept {
		return rhs.Id < lhs.Id;
	}

	/// <summary>
	/// Operator overloading for use in unordered map.
	/// </summary>
	/// <param name="lhs">The first entity.</param>
	/// <param name="rhs">Theother entity to compare to.</param>
	/// <returns></returns>
	bool operator>(const Entity& lhs, const Entity& rhs) noexcept {
		return rhs.Id > lhs.Id;
	}
}

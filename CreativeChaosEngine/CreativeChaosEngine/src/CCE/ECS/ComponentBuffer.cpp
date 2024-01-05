#include "ComponentBuffer.h"
#include "../Analysis/Debug.h"
#include "EntityComponentSystem.h"

namespace CCE::ECS
{
	/// <summary>
	/// Add new Component data to the component array.
	/// </summary>
	/// <param name="entity">The entity to insert the data to.</param>
	/// <param name="component">The component to add.</param>
	template<typename T>
	void ComponentBuffer<T>::InsertData(Entity entity, T component)
	{
		DASSERT(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end(),
			"Component added to same entity more than once.");

		UINT64 newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	/// <summary>
	/// Removes the components data.
	/// </summary>
	/// <param name="entity">The entity associated with the component.</param>
	template<typename T>
	void ComponentBuffer<T>::RemoveData(Entity entity)
	{
		DASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(),
			"Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		UINT64 indexOfRemovedEntity = mEntityToIndexMap[entity];
		UINT64 indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}	

	/// <summary>
	/// Returns the data from the entity.
	/// </summary>
	/// <param name="entity">The entity associated with the component data.</param>
	/// <returns>The component data.</returns>
	template<typename T>
	T& ComponentBuffer<T>::GetData(Entity entity)
	{
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	/// <summary>
	/// The virtual function for notifying the buffer when an entity was destroyed.
	/// </summary>
	/// <param name="entity">The entity which was destroyed.</param>
	template<typename T>
	void ComponentBuffer<T>::EntityDestroyed(Entity entity)
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}
}

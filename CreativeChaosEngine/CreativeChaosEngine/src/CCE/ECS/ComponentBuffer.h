#pragma once
#include "IComponentBuffer.h"
#include "../Analysis/Debug.h"
#include <unordered_map>
#include <array>

namespace CCE::ECS
{
	template<typename T>
	class ComponentBuffer : public IComponentBuffer
	{
	public:
		ComponentBuffer() : IComponentBuffer() {}
		~ComponentBuffer() {}
		
		/// <summary>
		/// Add new Component data to the component array.
		/// </summary>
		/// <param name="entity">The entity to insert the data to.</param>
		/// <param name="component">The component to add.</param>
		void InsertData(UINT64 entity, T component)
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
		void RemoveData(UINT64 entity)
		{
			DASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(),
				"Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			UINT64 indexOfRemovedEntity = mEntityToIndexMap[entity];
			UINT64 indexOfLastElement = mSize - 1;
			mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

			// Update map to point to moved spot
			UINT64 entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
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
		T* GetData(UINT64 entity)
		{
			if (UINT64 index = mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				return &mComponentArray.at(index);
			}
			return nullptr;
		}

		/// <summary>
		/// The virtual function for notifying the buffer when an entity was destroyed.
		/// </summary>
		/// <param name="entity">The entity which was destroyed.</param>
		virtual void EntityDestroyed(UINT64 entity) override
		{
			if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
			{
				RemoveData(entity);
			}
		}

	private:

		struct EntityHasher
		{
			size_t operator()(const UINT64& other) const noexcept
			{
				return static_cast<size_t>(other);
			}
		};

		std::array<T, 4096> mComponentArray = {};
		std::unordered_map<UINT64, UINT64, EntityHasher> mEntityToIndexMap = {};
		std::unordered_map<UINT64, UINT64> mIndexToEntityMap = {};
		UINT64 mSize = 0;
	};
}
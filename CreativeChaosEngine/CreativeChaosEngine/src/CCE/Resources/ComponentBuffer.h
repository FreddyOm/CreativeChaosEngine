#pragma once
#include "IComponentBuffer.h"
#include <unordered_map>
#include <array>

namespace CCE::Resources
{
	template<typename T>
	class ComponentBuffer
	{
	public:
		ComponentBuffer() : IComponentBuffer() {}
		~ComponentBuffer() = default;

		void InsertData(Entity entity, T component);
		void RemoveData(Entity entity);

		T& GetData(Entity entity);
		virtual void EntityDestroyed(Entity entity) override;

	private:

		std::array<T, 4096> mComponentArray = {};
		std::unordered_map<Entity, UINT64> mEntityToIndexMap = {};
		std::unordered_map<UINT64, Entity> mIndexToEntityMap = {};
		UINT64 mSize = 0;
	};
}
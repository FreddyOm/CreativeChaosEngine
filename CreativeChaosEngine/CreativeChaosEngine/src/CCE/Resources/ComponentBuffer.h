#pragma once
#include "IComponentBuffer.h"
#include <unordered_map>
#include <array>

namespace CCE::Resources
{
	template<typename T>
	class ComponentBuffer : IComponentBuffer
	{
	public:
		ComponentBuffer() : IComponentBuffer() {}
		~ComponentBuffer() {}

		void InsertData(Entity entity, T component);
		void RemoveData(Entity entity);

		T& GetData(Entity entity);
		virtual void EntityDestroyed(Entity entity) override;
	private:

		struct EntityHasher
		{
			size_t operator()(const Entity& other) const noexcept
			{
				return static_cast<size_t>(other.Id);
			}
		};

		std::array<T, 4096> mComponentArray = {};
		std::unordered_map<Entity, UINT64, EntityHasher> mEntityToIndexMap = {};
		std::unordered_map<UINT64, Entity> mIndexToEntityMap = {};
		UINT64 mSize = 0;
	};
}
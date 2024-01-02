#pragma once
#include "../Core.h"
#include "../String/String.h"

namespace CCE::Resources
{
	struct Entity
	{
	public:
		Entity() {};
		Entity(UINT64 id)
			: Id(id)
		{}

		Entity(String name)
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

		bool operator<(const Entity& other)
		{
			return Id < other.Id;
		}

		bool operator>(const Entity& other)
		{
			return Id > other.Id;
		}

		template<typename T>
		void AddComponent();

		template<typename T>
		void RemoveComponent();

		template<typename T>
		T& GetComponent();

		template<typename T>
		bool TryGetComponent(T& outRef);

	public:
		UINT64 Id = 0;
	};
}

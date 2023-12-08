#pragma once
#include "../Core.h"
#include "../String/String.h"

namespace CCE::Resources
{
	struct CCE_API Entity
	{
	public:
		Entity() {};
		Entity(UINT64 id)
			: Id(id)
		{}

		Entity(String name)
			: Id(name.sId)
		{}

		~Entity() = default;		

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

/*
#pragma once
#include "../Core.h"

namespace CCE::Resources
{

	template<typename C>
	struct CCE_API Component
	{
		using ComponentType = DWORD;

	public:
		Component(DWORD type)
			: Type(type)
		{ }
		~Component() = default;

		C* GetResource() const
		{
			return &resource;
		}

		UINT64 GetEntityHandle() const
		{
			return entityHandle;
		}

	public:

		const ComponentType Type = 0;

	private:

		UINT64 entityHandle = 0;
		C resource;
	};
}

*/

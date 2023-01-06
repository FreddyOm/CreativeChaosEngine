#pragma once
#include "../Core.h"

namespace CCE
{
	struct CCE_API BaseManager
	{
	public:
		inline BaseManager() noexcept {}
		inline ~BaseManager() noexcept {}

		virtual void StartUp() = 0;
		virtual void ShutDown() = 0;
	protected:
		bool initialized = false;
	};
}

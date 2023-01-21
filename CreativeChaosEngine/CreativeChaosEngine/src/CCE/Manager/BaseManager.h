#pragma once
#include "../Core.h"

namespace CCE
{
	struct CCE_API BaseManager
	{
	public:
		BaseManager() noexcept {}
		~BaseManager() noexcept {}

		virtual void StartUp() = 0;
		virtual void ShutDown() = 0;
	protected:
		bool initialized = false;
	};
}

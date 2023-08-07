#pragma once
#include "../../Core.h"
#include <atomic>

namespace CCE
{
	struct CCE_API SpinLock
	{
		SpinLock() = default;
		~SpinLock() = default;

		bool Lock()
		{
			return lock.test_and_set();
		}

		void Unlock()
		{
			lock.clear();
		}

	private:
		std::atomic_flag lock = ATOMIC_FLAG_INIT;
	};
}
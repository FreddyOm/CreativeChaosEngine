#pragma once
#include "../../Core.h"
#include "../../Analysis/Logger.h"
#include <atomic>

namespace CCE
{
	struct CCE_API SpinLock
	{
		SpinLock() = default;
		~SpinLock() = default;

		void Acquire()
		{
			for (;;) {
				// Returns false when it switched from true to false!
				if (!lock.exchange(true, std::memory_order_acquire)) {
					break;
				}
			}
		}

		void Release()
		{
			lock.store(false, std::memory_order_release);
		}

	private:
		std::atomic<bool> lock = {false};
	};
}
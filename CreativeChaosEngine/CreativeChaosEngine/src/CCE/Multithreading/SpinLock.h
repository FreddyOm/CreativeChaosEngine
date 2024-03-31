#pragma once
#include "../core.h"
#include <atomic>

namespace CCE
{
	struct SpinLock
	{
		SpinLock() = default;
		~SpinLock() = default;

		__forceinline void Acquire() const
		{
			for (;;) {
				// Returns false when it switched from true to false!
				if (!lock.exchange(true, std::memory_order_acquire)) {
					break;
				}
			}
		}

		__forceinline void Release() const
		{
			lock.store(false, std::memory_order_release);
		}

	private:
		mutable std::atomic<bool> lock = {false};
	};
}
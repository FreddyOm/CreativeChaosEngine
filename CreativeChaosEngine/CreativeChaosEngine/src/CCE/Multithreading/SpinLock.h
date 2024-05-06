#pragma once
#include "../core.h"
#include "../analysis/Debug.h"
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

			++lockCount;
			DASSERT(lockCount == 1, "Lock was aquired by more than one user at the same time!");
		}

		__forceinline void Release() const
		{
			lock.store(false, std::memory_order_release);
		}

	private:
		mutable std::atomic<bool> lock = { false };
		mutable std::atomic<int> lockCount = {0};
	};
}
#pragma once
#include "../core.h"
#include "../analysis/Debug.h"
#include <atomic>
#include <mutex>

namespace CCE
{
	struct SpinLock
	{
		SpinLock() = default;
		~SpinLock() = default;

		__forceinline void Acquire() const noexcept
		{
			// Guard
			while (lock.test_and_set(std::memory_order_seq_cst))
				lock.wait(true, std::memory_order_relaxed);
		}

		__forceinline void Release() const noexcept
		{
			lock.clear(std::memory_order_release);
			lock.notify_one();
		}

	private:
		mutable std::atomic_flag lock = ATOMIC_FLAG_INIT;
	};
}
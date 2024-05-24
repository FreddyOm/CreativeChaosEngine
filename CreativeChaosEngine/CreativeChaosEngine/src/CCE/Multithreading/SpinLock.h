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

		__forceinline void Acquire() const
		{
			//printf("Acquiring Mutex on Thread %d\n", GetCurrentThreadId());
			std::lock_guard<std::mutex> guard(aquireLockMutex);

			// Guard
			while (lock.load(std::memory_order_seq_cst)) 
			{ }
			lock.store(true, std::memory_order_seq_cst);
		}

		__forceinline void Release() const
		{
			//printf("Releasing Mutex on Thread %d\n", GetCurrentThreadId());
			std::lock_guard<std::mutex> guard(releaseLockMutex);
			lock.store(false, std::memory_order_seq_cst);
		}

	private:
		mutable std::atomic<bool> lock = { false };
		mutable std::mutex aquireLockMutex{};
		mutable std::mutex releaseLockMutex{};
	};
}
#pragma once
#include "../../Core.h"
#include "SpinLock.h"

namespace CCE 
{
	struct CCE_API ScopedSpinLock
	{
		ScopedSpinLock(SpinLock& _spinlock)
		{
			spinlock = &_spinlock;
			while (!spinlock->Lock())
			{
				continue;
			}
		}

		~ScopedSpinLock()
		{
			spinlock->Unlock();
			spinlock = nullptr;
		}

	private:
		SpinLock* spinlock = nullptr;
	};
}

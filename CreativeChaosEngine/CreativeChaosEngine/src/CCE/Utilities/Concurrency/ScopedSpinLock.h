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
			spinlock->Acquire();
		}

		~ScopedSpinLock()
		{
			spinlock->Release();
			spinlock = nullptr;
		}

	private:
		SpinLock* spinlock = nullptr;
	};
}

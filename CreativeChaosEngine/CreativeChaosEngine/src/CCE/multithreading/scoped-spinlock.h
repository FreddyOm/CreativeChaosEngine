#pragma once
#include "../core.h"
#include "spinLock.h"
#include "../../Thirdparty/src/optick.h"

namespace CCE 
{
	struct ScopedSpinLock
	{
		ScopedSpinLock(SpinLock& _spinlock)
		{
			OPTICK_EVENT();
			spinlock = &_spinlock;
			spinlock->Acquire();
		}

		~ScopedSpinLock()
		{
			OPTICK_EVENT();
			spinlock->Release();
			spinlock = nullptr;
		}

	private:
		SpinLock* spinlock = nullptr;
	};
}

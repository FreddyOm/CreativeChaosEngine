#pragma once
#include "../core.h"
#include "spinLock.h"
#include "../analysis/Debug.h"
#include "../../Thirdparty/src/optick.h"

namespace CCE 
{
	struct ScopedSpinLock
	{
		ScopedSpinLock(SpinLock& const _spinlock)
		{
			spinlock = &_spinlock;
			spinlock->Acquire();

#if defined( DEBUG ) || defined ( DEBUG_PROFILE )
			threadId = GetThreadId(GetCurrentThread());	// For checking the thread that aquired the spinlock!
#endif
		}

		~ScopedSpinLock()
		{
			spinlock->Release();
			spinlock = nullptr;
#if defined( DEBUG ) || defined ( DEBUG_PROFILE )
			threadId = 0;
#endif
		}

	private:
		SpinLock* spinlock = nullptr;
#if defined( DEBUG ) || defined ( DEBUG_PROFILE )
		DWORD threadId = 0;
#endif
	};
}

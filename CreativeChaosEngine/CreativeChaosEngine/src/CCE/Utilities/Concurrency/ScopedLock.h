#pragma once
#include "../../Core.h"
#include<mutex>
namespace CCE
{
	struct CCE_API ScopedLock
	{
		ScopedLock(std::mutex* mutex)
		{
			m = mutex;
			m->lock();
		}
		~ScopedLock()
		{
			m->unlock();
			m = nullptr;
		}
	private:
		std::mutex* m;
	};
}

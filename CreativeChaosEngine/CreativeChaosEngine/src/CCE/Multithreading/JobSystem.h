#pragma once
#include "../Core.h"
#include <thread>
#include <vector>
#include <atomic>


namespace CCE::Jobs
{
	void CCE_API InitializeThreadpool(int numOfThreads = -1);
	void CCE_API DeinitializeThreadpool();

	void RunThread();
}

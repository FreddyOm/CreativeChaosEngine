#pragma once
#include "../core.h"
#include <thread>
#include <vector>
#include <atomic>
#include "job.h"


namespace CCE::Jobs
{
	void CCE_API InitializeThreadpool(int numOfThreads = -1);
	void CCE_API DeinitializeThreadpool();

	void RunThread();

	void CCE_API BusyWaitForCounter(Counter* const cnt, const int desiredCount = 0);
	void CCE_API BusyWaitForCounterAndFree(Counter* const cnt, const int desiredCount = 0);


	void CCE_API KickJob(Job job);
	void CCE_API KickJobs(Job* jobs, int jobCount);
}

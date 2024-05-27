#pragma once
#include "../core.h"
#include <vector>
#include "job.h"


namespace CCE::Jobs
{
	CCE_API void InitializeThreadpool(int numOfThreads = -1);
	CCE_API void DeinitializeThreadpool();

	CCE_API void BusyWaitForCounter(Counter* const cnt, const int desiredCount = 0);
	CCE_API void BusyWaitForCounterAndFree(Counter* const cnt, const int desiredCount = 0);

	CCE_API void KickJob(Job job);
	CCE_API void KickJobs(Job* jobs, int jobCount);
	
	// @TODO: Implement specifically for main thread to preferably 
	// pick main thread jobs.
	
	// CCE_API void KickMainThreadJob(Job job);
	// CCE_API void KickMainThreadJobs(Job* job, int jobCount);

	void RunThread();
	void UpdateWaitData();
}

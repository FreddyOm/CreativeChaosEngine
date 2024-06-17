#pragma once
#include "../Core.h"
#include "JobProfilingData.h"
#include "JobProfilingCollector.h"
#include <vector>
#include <unordered_map>

#if defined(DEBUG_PROFILE)

#define SAMPLE_JOB() CCE::Debug::JobProfilingCollector sampler(__FUNCTION__, __FILE__, __LINE__)
#define RESET_SAMPLES() CCE::Debug::ResetFrameSamples()
#define PRE_SWITCH_FIBER() CCE::Debug::PreSwitchFiber()
#define POST_SWITCH_FIBER() CCE::Debug::PostSwitchFiber()

#else // DEBUG & RELEASE
#define SAMPLE_JOB()
#define RESET_SAMPLES()
#define PRE_SWITCH_FIBER()
#define POST_SWITCH_FIBER()
#endif // DEBUG & RELEASE

namespace CCE::Debug
{
	CCE_API extern bool sampleJobs;
	CCE_API extern Time::time::time_point frameStart;

	CCE_API void InitializeJobProfiler();
	CCE_API void RegisterJobData(JobProfilingData&& data);
	CCE_API void ResetFrameSamples();
	CCE_API void SetJobExitTime(Time::time::time_point exitTime);

	CCE_API std::unordered_map<DWORD, std::vector<JobProfilingData>>& GetFrameProfilingData2();

	void SetCurrentProfilingData(JobProfilingData* data);
	void PreSwitchFiber();
	void PostSwitchFiber();
}

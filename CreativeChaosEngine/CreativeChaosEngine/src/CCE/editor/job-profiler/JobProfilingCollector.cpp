#include "JobProfilingCollector.h"
#include "jobProfiler.h"

namespace CCE::Debug
{
	JobProfilingCollector::JobProfilingCollector(std::string funcName, std::string fileName, unsigned int lineNum)
		: jobEntryTime(Time::Now())
		, functionName(std::move(funcName))
		, fileName(std::move(fileName))
		, lineNumber(lineNum)
		, jobFiber(GetCurrentFiber())
	{
		jobThreadId.push_back(GetCurrentThreadId());
		RegisterJobData(JobProfilingData(std::move(*this)));
	}

	JobProfilingCollector::~JobProfilingCollector()
	{
		jobExitTime = Time::Now();
		SetJobExitTime(jobExitTime);
	}
}
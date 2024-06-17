#include "JobProfilingData.h"
#include "JobProfilingCollector.h"

namespace CCE::Debug
{
	JobProfilingData::JobProfilingData() {}

	JobProfilingData::JobProfilingData(JobProfilingCollector&& collectedData)
		: jobEntryTime(collectedData.jobEntryTime)
		, jobExitTime(collectedData.jobExitTime)
		, functionName(std::move(collectedData.functionName))
		, fileName(std::move(collectedData.fileName))
		, lineNumber(collectedData.lineNumber)
		, jobFiber(collectedData.jobFiber)
		, profilingWaitData(std::move(collectedData.profilingWaitData))
		, jobThreadId(std::move(collectedData.jobThreadId))
	{ }
}

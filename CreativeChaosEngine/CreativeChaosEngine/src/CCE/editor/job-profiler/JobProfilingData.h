#pragma once
#include "../Core.h"
#include "../analysis/Time.h"
#include <vector>

namespace CCE::Debug
{
	extern class JobProfilingCollector;
	extern class JobProfilingWaitData;

	struct CCE_API JobProfilingData
	{
		JobProfilingData();
		JobProfilingData(JobProfilingCollector&& collectedData);

		Time::time::time_point jobEntryTime{};
		Time::time::time_point jobExitTime{};

		std::string functionName{};
		std::string fileName{};
		unsigned int lineNumber{};

		LPVOID jobFiber{};
		std::vector<JobProfilingWaitData> profilingWaitData{};
		std::vector<DWORD> jobThreadId{};
	};
}

#pragma once
#include "../Core.h"
#include <string>
#include "../analysis/Time.h"
#include "JobProfilingWaitData.h"

namespace CCE::Debug
{
	struct CCE_API JobProfilingCollector
	{
		JobProfilingCollector(std::string funcName, std::string fileName, unsigned int lineNum);

		~JobProfilingCollector();

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

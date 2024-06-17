#pragma once
#include "../analysis/Time.h"

namespace CCE::Debug
{
	struct JobProfilingWaitData
	{
		JobProfilingWaitData()
			: jobWaitEntryTime(Time::Now())
		{ }

		Time::time::time_point jobWaitEntryTime{};
		Time::time::time_point jobWaitExitTime{};
	};
}
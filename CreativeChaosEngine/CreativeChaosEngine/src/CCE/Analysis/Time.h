#pragma once
#include "../Core.h"
#include <chrono>

namespace CCE
{
	namespace cr = std::chrono;

	struct CCE_API Time
	{
		using time = cr::steady_clock;
		using floatsec = cr::duration<float>;
		using time_point = cr::time_point<time, floatsec>;

		static time_point CurrentTick();
	};

	struct CCE_API DateTime
	{
		static char currentTime[8];
		static const char* GetTime();
	};

}

// TODO: Change API so other classes can get the milliseconds / clockcycles since init
// and can easily calculate execution time like that.
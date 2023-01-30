#pragma once
#include "../Core.h"
#include <chrono>

namespace CCE
{
	namespace cr = std::chrono;

	struct CCE_API Time
	{
		using time = cr::high_resolution_clock;
		using mics = cr::duration<cr::microseconds>;
		using time_point = cr::time_point<time, mics>;

		static cr::high_resolution_clock::time_point CurrentTick();
		static double GetDurationInMilliSec(const cr::high_resolution_clock::time_point start,
			const cr::high_resolution_clock::time_point end);
		static long GetDurationInMicroSec(const cr::high_resolution_clock::time_point start,
			const cr::high_resolution_clock::time_point end);
	};

	struct CCE_API DateTime
	{
		static char currentTime[8];
		static const char* GetTime();
	};

}
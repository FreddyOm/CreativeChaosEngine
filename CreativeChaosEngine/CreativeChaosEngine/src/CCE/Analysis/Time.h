#pragma once
#include "../Core.h"
#include <chrono>

namespace CCE
{
	namespace cr = std::chrono;

	struct CCE_API Time
	{
#define AVG_BUF_LEN 14

		using time = cr::high_resolution_clock;
		using mics = cr::duration<cr::microseconds>;
		using time_point = cr::time_point<time, mics>;

		alignas(16) static double average[AVG_BUF_LEN];
		static double deltaTime;
	private:
		static short averageIndex;

	public:
		static time::time_point Now();
		static time::time_point CurrentTick();
		static double GetDurationInMilliSec(const time::time_point start, const time::time_point end);
		static long GetDurationInMicroSec(const time::time_point start, const time::time_point end);
		static void UpdateDeltaTime(const double millis);
		static double GetAverageFrameTime();
	};

	struct CCE_API DateTime
	{
		alignas(16) static char currentTime[8];		// 8 bytes
		static const char* GetTime();
	};

}
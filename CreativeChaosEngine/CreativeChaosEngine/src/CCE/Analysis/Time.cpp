#include "Time.h"
#include "time.h"

namespace CCE
{
	std::chrono::high_resolution_clock::time_point Time::CurrentTick()
	{
		using time = cr::high_resolution_clock;
		return time::now();
	}

	double Time::GetDurationInMilliSec(const cr::high_resolution_clock::time_point start, const cr::high_resolution_clock::time_point end)
	{
		auto duration = cr::duration_cast<cr::microseconds>(end - start);
		return (double)duration.count() / 1000.0;
	}

	long Time::GetDurationInMicroSec(const cr::high_resolution_clock::time_point start, const cr::high_resolution_clock::time_point end)
	{
		auto duration = cr::duration_cast<cr::microseconds>(end - start);
		return (long) duration.count();
	}

	char DateTime::currentTime[8];

	const char* DateTime::GetTime()
	{
		char buffer[26];
		time_t now = time(0);
		ctime_s(buffer, sizeof(buffer), &now);


		for (size_t i = 0; i < 8; i++)
		{
			currentTime[i] = buffer[i + 11];
		}

		return currentTime;
	}
}
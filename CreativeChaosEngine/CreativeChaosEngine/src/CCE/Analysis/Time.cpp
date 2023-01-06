#include "Time.h"
#include "time.h"

namespace CCE
{
	Time::time_point Time::CurrentTick()
	{
		using time = cr::steady_clock;
		return time::now();
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
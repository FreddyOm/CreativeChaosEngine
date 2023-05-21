#include "Time.h"
#include "time.h"

namespace CCE
{
	alignas(16) double Time::average[AVG_BUF_LEN] = { 16.6, 16.6, 16.6, 16.6, 16.6, 16.6,
		16.6, 16.6, 16.6, 16.6, 16.6, 16.6, 16.6, 16.6};
	double Time::deltaTime = 16.6;
	short Time::averageIndex = 0;

	Time::time::time_point Time::Now()
	{
		return time::now();
	}

	cr::high_resolution_clock::time_point Time::CurrentTick()
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

	void Time::SetDeltaTime(const double millis)
	{
		deltaTime = millis;

		if (millis > 1000)
		{
			deltaTime = 16.6;
		}

		average[averageIndex] = deltaTime;
		
		if (averageIndex < AVG_BUF_LEN)
		{
			++averageIndex;
		}
		else 
		{
			averageIndex = 0;
		}
	}

	double Time::GetAverageFrameTime()
	{
		double avg = 0;
		for (int i = 0; i < AVG_BUF_LEN; ++i)
		{
			avg += average[i] / AVG_BUF_LEN;
		}
		return avg;
	}


	#pragma region date time

	alignas(16) char DateTime::currentTime[8];

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

	#pragma endregion date time
}
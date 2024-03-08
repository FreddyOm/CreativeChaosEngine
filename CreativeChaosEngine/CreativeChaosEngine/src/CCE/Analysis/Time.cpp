#include "Time.h"
#include "time.h"
#include "../Manager/Application.h"

namespace CCE
{
	alignas(16) double Time::average[AVG_BUF_LEN] = { 0.016, 0.016, 0.016, 0.016, 0.016, 0.016,
		0.016, 0.016, 0.016, 0.016, 0.016, 0.016, 0.016, 0.016 };
	double Time::deltaTime = 0.016;
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

	long long Time::GetDurationInMicroSec(const cr::high_resolution_clock::time_point start, const cr::high_resolution_clock::time_point end)
	{
		auto duration = cr::duration_cast<cr::microseconds>(end - start);
		return (long long) duration.count();
	}

	void Time::SetDeltaTime(const double millis)
	{
		OPTICK_EVENT();
		deltaTime = millis / 1000.0;

		if (millis > 1000)
		{
			deltaTime = 0.016;
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

	long long Time::GetMillisSinceStart()
	{
		return cr::duration_cast<cr::milliseconds>(Now() - Application::Instance->startTime).count();
	}

	double Time::GetSecondsSinceStart()
	{
		return static_cast<double>(GetMillisSinceStart() / 1000.0);
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
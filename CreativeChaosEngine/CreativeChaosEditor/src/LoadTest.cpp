#include "LoadTest.h"
#include "CCE/Analysis/Logger.h"

void LoadTest::DoWork()
{
	for (int i = 0; i < 100; i++)
	{
		int a = i % 7;
		a += 3;
		if (a == 10)
			i++;
	}

	LOGC("Work was done!", COLOR_PINK);
}

void LoadTest::DoWorkWithArgs(int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		int a = i % 7;
		a += 3;
		if (a == 10)
			i++;
	}
	LOGC("Work with args was done!", COLOR_PINK);
}

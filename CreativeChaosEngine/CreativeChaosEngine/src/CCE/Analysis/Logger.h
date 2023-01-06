#pragma once
#include "../Core.h"
#include "Windows.h"

namespace CCE
{
#define COLOR unsigned char
#define COLOR_GREEN 10
#define COLOR_BLUE 11
#define COLOR_RED 12
#define COLOR_PINK 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

	struct CCE_API Logger
	{
	public:
		Logger() = default;
		~Logger() = default;
		static void Log(const char* msg, COLOR color = COLOR_WHITE);

	private:
		static HANDLE hConsole;
	};
}

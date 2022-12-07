#pragma once

#if _DEBUG
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <iostream>
#include <string>

namespace CCE_Debug
{

	struct DebugConsole
	{
		DebugConsole();
		~DebugConsole();

	private:
		static bool consoleRunning;
		static DWORD charsWritten;
		static int hConHandle;
		static long lStdHandle;
		static FILE* fp;

	public:

		static void ConsoleLog(std::wstring message, int color = 15);

		static void CloseConsole();
	};
}

#endif

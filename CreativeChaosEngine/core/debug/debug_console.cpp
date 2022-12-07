#include "debug_console.h"

#if _DEBUG
#include <sstream>

using namespace CCE_Debug;

DebugConsole::DebugConsole()
{
	if (consoleRunning)
		return;

	consoleRunning = true;

	AttachConsole(ATTACH_PARENT_PROCESS);
	AllocConsole();

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle((unsigned long)lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(fp, NULL, _IONBF, 1);

	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 128);

	// STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);

	std::ios_base::sync_with_stdio();
}

DebugConsole::~DebugConsole()
{
	CloseConsole();
}

bool DebugConsole::consoleRunning;
DWORD DebugConsole::charsWritten;
int DebugConsole::hConHandle;
long DebugConsole::lStdHandle;
FILE* DebugConsole::fp;

void DebugConsole::ConsoleLog(std::wstring message)
{
	int len = wcslen(message.c_str());
	WriteConsole((HANDLE)lStdHandle, message.c_str(), len, &charsWritten, NULL);
	WriteConsole((HANDLE)lStdHandle, L"\n", 1, &charsWritten, NULL);
	charsWritten += len;
}

void DebugConsole::CloseConsole()
{
	FreeConsole();
	free(fp);
	consoleRunning = false;
}

#endif

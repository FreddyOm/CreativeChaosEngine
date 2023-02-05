#include "Debug.h"
#include "Logger.h"
#include <stdio.h>

namespace CCE
{
	void Debug::DebugInfo(const char* msg, const char* file, const int line) noexcept
	{
		char buf[8 + 256] = { 0 };
		char line_buf[5] = { 0 };
		
		strcpy_s(buf, infoTag);
		strcat_s(buf, msg);
#if 0
		strcat_s(buf, "\nFile: ");
		strcat_s(buf, file);
		strcat_s(buf, "\nLine: ");
		sprintf_s(line_buf, "%i", line);
		strcat_s(buf, line_buf);
#endif
		CCE::Logger::Log(buf);
	}

	void Debug::DebugWarning(const char* msg, const char* file, const int line) noexcept
	{
		char buf[11 + 256] = { 0 };
		char line_buf[5] = { 0 };

		strcpy_s(buf, warningTag);
		strcat_s(buf, msg);

		strcat_s(buf, "\nFile: ");
		strcat_s(buf, file);
		strcat_s(buf, "\nLine: ");
		sprintf_s(line_buf, "%i", line);
		strcat_s(buf, line_buf);

		CCE::Logger::Log(buf, COLOR_YELLOW);
	}

	void Debug::DebugError(const char* msg, const char* file, const int line) noexcept
	{
		char buf[9 + 256] = { 0 };
		char line_buf[5] = { 0 };

		strcpy_s(buf, errorTag);
		strcat_s(buf, msg);

		strcat_s(buf, "\nFile: ");
		strcat_s(buf, file);
		strcat_s(buf, "\nLine: ");
		sprintf_s(line_buf, "%i", line);
		strcat_s(buf, line_buf);

		CCE::Logger::Log(buf, COLOR_RED);
	}

	void Debug::DebugAssert(const bool condition, const char* msg, const char* file, const int line) noexcept
	{
		if (condition)
			return;

		char buf[9 + 256] = { 0 };
		char line_buf[5] = { 0 };

		strcpy_s(buf, assertTag);
		strcat_s(buf, msg);

		strcat_s(buf, "\nFile: ");
		strcat_s(buf, file);
		strcat_s(buf, "\nLine: ");
		sprintf_s(line_buf, "%i", line);
		strcat_s(buf, line_buf);

		MessageBoxA(NULL, buf, "Assertion", MB_OK | MB_ICONERROR);
		exit(-1);

		//TODO: Write all asserts to an assert file on disk so one can
		// easily track where the assertion ocurred even after the program
		// was closed
	}

	const char* Debug::infoTag = "[INFO]: ";
	const char* Debug::warningTag  = "[WARNING]: ";
	const char* Debug::errorTag = "[ERROR]: ";
	const char* Debug::assertTag = "[ASSERT]: ";
}

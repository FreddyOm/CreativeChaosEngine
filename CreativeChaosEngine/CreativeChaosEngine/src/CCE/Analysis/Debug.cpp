#include "Debug.h"
#include "Logger.h"

namespace CCE
{
	void Debug::DebugInfo(const char* msg, const char* file, const int line) noexcept
	{
		CCE::Logger::Log("[INFO] %s\n%s\n%i", COLOR_WHITE, CCE::NONE, msg, file, line);
	}

	void Debug::DebugWarning(const char* msg, const char* file, const int line) noexcept
	{
		CCE::Logger::Log("[WARNING] %s\n%s\n%i", COLOR_YELLOW, CCE::NONE, msg, file, line);
	}

	void Debug::DebugError(const char* msg, const char* file, const int line) noexcept
	{
		CCE::Logger::Log("[ERROR] %s\n%s\n%i", COLOR_RED, CCE::NONE, msg, file, line);
	}

	void Debug::DebugAssert(const bool condition, const char* msg, const char* file, const int line) noexcept
	{
		if (condition)
			return;

		char buf[9 + 256] = { 0 };
		char line_buf[5] = { 0 };

		strcpy_s(buf, "[ASSERT] ");
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
}

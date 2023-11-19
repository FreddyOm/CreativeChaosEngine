#include "Debug.h"
#include "Logger.h"

namespace CCE
{
	void Debug::DebugInfo(const char* msg, const char* file, const int line) noexcept
	{
		auto desc = Logger::DebugInfoDesc{ msg, file, line, LogLevel::NONE };
		CCE::Logger::Log(desc);
	}

	void Debug::DebugWarning(const char* msg, const char* file, const int line) noexcept
	{
		std::string msgStr = std::string("[WARNING] ");
		msgStr += msg;
		auto desc = Logger::DebugInfoDesc{ msgStr, file, line, LogLevel::WARNING };
		CCE::Logger::Log(desc);
	}

	void Debug::DebugError(const char* msg, const char* file, const int line) noexcept
	{
		std::string msgStr = std::string("[ERROR] ");
		msgStr += msg;
		auto desc = Logger::DebugInfoDesc{ msgStr, file, line, LogLevel::ERR };
		CCE::Logger::Log(desc);
	}

	void Debug::DebugError(const DWORD error, const char* file, const int line) noexcept
	{
		if (error == 0) {
			return; //No error message has been recorded
		}

		LPSTR messageBuffer = nullptr;
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US), 
			(LPSTR)&messageBuffer, 0, NULL);

		DebugError(messageBuffer, file, line);

		LocalFree(messageBuffer);

		DebugBreak();
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
		DebugBreak();
		//exit(-1);

		//TODO: Write all asserts to an assert file on disk so one can
		// easily track where the assertion ocurred even after the program
		// was closed
	}
}

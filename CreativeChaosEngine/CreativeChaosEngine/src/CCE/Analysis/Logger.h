#pragma once
#include <mutex>
#include "../core.h"
#include "../string/string.h"
#include "../multithreading/scoped-spinlock.h"
#include "../multithreading/spinlock.h"
#include "time.h"

#ifdef DEBUG

#define LOG(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::NONE, ##__VA_ARGS__)
#define LOGC(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::NONE, ##__VA_ARGS__)

#define LOG_TEST(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::TEST, ##__VA_ARGS__)
#define LOGC_TEST(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::TEST, ##__VA_ARGS__)

#define LOG_INPUT(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::INPUT, ##__VA_ARGS__)
#define LOGC_INPUT(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::INPUT, ##__VA_ARGS__)

#define LOG_REND(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::RENDERING, ##__VA_ARGS__)
#define LOGC_REND(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::RENDERING, ##__VA_ARGS__)

#define LOG_JOBS(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::JOBS, ##__VA_ARGS__)
#define LOGC_JOBS(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::JOBS, ##__VA_ARGS__)

#define LOG_PROFILING(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::PROFILING, ##__VA_ARGS__)
#define LOGC_PROFILING(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::PROFILING, ##__VA_ARGS__)

#endif // DEBUG

#ifdef DEBUG_PROFILE
#define LOG(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::NONE, ##__VA_ARGS__)
#define LOGC(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::NONE, ##__VA_ARGS__)

#define LOG_TEST(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::TEST, ##__VA_ARGS__)
#define LOGC_TEST(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::TEST, ##__VA_ARGS__)

#define LOG_INPUT(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::INPUT, ##__VA_ARGS__)
#define LOGC_INPUT(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::INPUT, ##__VA_ARGS__)

#define LOG_REND(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::RENDERING, ##__VA_ARGS__)
#define LOGC_REND(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::RENDERING, ##__VA_ARGS__)

#define LOG_JOBS(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::JOBS, ##__VA_ARGS__)
#define LOGC_JOBS(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::JOBS, ##__VA_ARGS__)

#define LOG_PROFILING(msg, ...) CCE::Logger::Log(msg, COLOR_WHITE, CCE::LogLevel::PROFILING, ##__VA_ARGS__)
#define LOGC_PROFILING(msg, c, ...) CCE::Logger::Log(msg, c, CCE::LogLevel::PROFILING, ##__VA_ARGS__)

#endif // PROFILE

#ifdef NDEBUG
#define LOG(msg)
#define LOGC(msg, c)

#define LOG_TEST(msg, ...) 
#define LOGC_TEST(msg, c, ...) 

#define LOG_INPUT(msg, ...)
#define LOGC_INPUT(msg, c, ...)

#define LOG_REND(msg, ...)
#define LOGC_REND(msg, c, ...)

#define LOG_JOBS(msg, ...)
#define LOGC_JOBS(msg, c, ...)

#define LOG_PROFILING(msg, ...)
#define LOGC_PROFILING(msg, c, ...)

#endif // RELEASE

namespace CCE
{
#define COLOR unsigned char
#define COLOR_GREEN 10
#define COLOR_BLUE 11
#define COLOR_RED 12
#define COLOR_PINK 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

	enum class LogLevel
	{
		NONE = 0,
		WARNING = 1,
		ERR = 2,
		INPUT = 3,
		RENDERING = 4,
		TEST = 5,
		JOBS = 6,
		PROFILING = 7,
	};

	struct CCE_API Logger
	{
		struct DebugInfoDesc // 24 byte
		{
			DebugInfoDesc(const std::string msg, const std::string file, const int line, const LogLevel type)
				: line(line), debugType(type), fileName(file)
			{
				this->msg = "[";
				this->msg += DateTime::GetTime();
				this->msg += "] " + msg;
			}

			int line = 0;
			LogLevel debugType = LogLevel::NONE;
			std::string fileName = "";
			std::string msg = "";
		};

		Logger() = default;
		~Logger() = default;
		static void Log(const char* msg, const COLOR color, const LogLevel level, ...);
		static void Log(const String msg, const COLOR color, const LogLevel level, ...);
		static void Log(const DebugInfoDesc desc);

		static void SetLogLvlMaks(const DWORD mask)
		{
			logLvLFilterMask = mask;
		}

		// @TODO: Probably move this out of logger and combine this with some other 
		// system to hook into an event.
		static std::vector<DebugInfoDesc> logBuffer;
		static void ClearDebugBuffer();
		static size_t logCount[3];

	private:
		static bool LogLvlActive(const LogLevel msgLogLvl)
		{
			return (logLvLFilterMask & 1 << (int)msgLogLvl) != 0;
		}
		static void CapDebugBuffer();

	private:
		static SpinLock logSpinLock;
		static HANDLE hConsole;					            // 8 bytes
		static DWORD logLvLFilterMask; // 0b00000001		// 8 bytes		
	};
}

// @TODO: Maybe log everything into a file ..?
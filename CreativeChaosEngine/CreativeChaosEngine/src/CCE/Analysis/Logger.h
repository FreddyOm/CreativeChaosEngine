#pragma once
#include <mutex>
#include "../Core.h"
#include "../String/String.h"

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

	enum LogLevel
	{
		NONE = 0,
		INPUT = 1,
		RENDERING = 2,
		TEST = 3,
		JOBS = 4,
	};

	struct CCE_API Logger
	{
		Logger() = default;
		~Logger() = default;
		static void Log(const char* msg, const COLOR color, const LogLevel level, ...);
		static void Log(const String msg, const COLOR color, const LogLevel level, ...);

		static void SetLogLvlMaks(const DWORD mask)
		{
			logLvLFilterMask = mask;
		}

	private:
		static bool LogLvlActive(const LogLevel msgLogLvl)
		{
			return (logLvLFilterMask & 1 << (int)msgLogLvl) != 0;
		}

	private:
		static std::mutex logMutex;
		static HANDLE hConsole;								// 8 bytes
		static DWORD logLvLFilterMask; // 0b00000001		// 8 bytes
	};
}

// TODO: Maybe log everything into a file ..?
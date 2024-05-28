#pragma once
#include <mutex>
#include "../core.h"
#include <vector>

#ifdef DEBUG

#define LOG(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::NONE, ##__VA_ARGS__)
#define LOGC(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::NONE, ##__VA_ARGS__)

#define LOG_TEST(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::TEST, ##__VA_ARGS__)
#define LOGC_TEST(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::TEST, ##__VA_ARGS__)

#define LOG_INPUT(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::INPUT, ##__VA_ARGS__)
#define LOGC_INPUT(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::INPUT, ##__VA_ARGS__)

#define LOG_REND(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::RENDERING, ##__VA_ARGS__)
#define LOGC_REND(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::RENDERING, ##__VA_ARGS__)

#define LOG_JOBS(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::JOBS, ##__VA_ARGS__)
#define LOGC_JOBS(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::JOBS, ##__VA_ARGS__)

#define LOG_PROFILING(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::PROFILING, ##__VA_ARGS__)
#define LOGC_PROFILING(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::PROFILING, ##__VA_ARGS__)

#endif // DEBUG

#ifdef DEBUG_PROFILE
#define LOG(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::NONE, ##__VA_ARGS__)
#define LOGC(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::NONE, ##__VA_ARGS__)

#define LOG_TEST(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::TEST, ##__VA_ARGS__)
#define LOGC_TEST(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::TEST, ##__VA_ARGS__)

#define LOG_INPUT(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::INPUT, ##__VA_ARGS__)
#define LOGC_INPUT(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::INPUT, ##__VA_ARGS__)

#define LOG_REND(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::RENDERING, ##__VA_ARGS__)
#define LOGC_REND(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::RENDERING, ##__VA_ARGS__)

#define LOG_JOBS(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::JOBS, ##__VA_ARGS__)
#define LOGC_JOBS(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::JOBS, ##__VA_ARGS__)

#define LOG_PROFILING(msg, ...) CCE::Log(msg, COLOR_WHITE, CCE::LogLevel::PROFILING, ##__VA_ARGS__)
#define LOGC_PROFILING(msg, c, ...) CCE::Log(msg, c, CCE::LogLevel::PROFILING, ##__VA_ARGS__)

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

// @TODO: Maybe log everything into a file ..?

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
	
	struct DebugInfoDesc;
	struct String;

	CCE_API extern size_t g_logCount[3];
	CCE_API extern std::vector<DebugInfoDesc> g_logBuffer;

	CCE_API void InitializeLogger();
	CCE_API void DeinitializeLogger();

	CCE_API void Log(const char* msg, const COLOR color, const LogLevel level, ...);
	CCE_API void Log(const String msg, const COLOR color, const LogLevel level, ...);

	CCE_API void SetLogLvlMaks(const DWORD mask);
	CCE_API bool LogLvlActive(const LogLevel msgLogLvl);
	CCE_API void ClearDebugBuffer();

	void Log(const DebugInfoDesc desc);
	void CapDebugBuffer();
}


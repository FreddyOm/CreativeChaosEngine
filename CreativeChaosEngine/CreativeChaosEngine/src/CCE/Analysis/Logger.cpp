#include "Logger.h"

namespace CCE
{
    /// <summary>
    /// Log a formatted message.
    /// </summary>
    /// <param name="msg">The formatted message.</param>
    /// <param name="color">The color code for the message.</param>
    /// <param name="level">The log level.</param>
    /// <param name="">additional arguments</param>
    void Logger::Log(const char* msg, const COLOR color = COLOR_WHITE, const LogLevel level = LogLevel::NONE, ...)
    {
        auto lock = ScopedSpinLock(logSpinLock);
        if (!LogLvlActive(level)) { return; }

        static char s_buffer[1024];

        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTitleA("CCE Debug Console");
        }

        SetConsoleTextAttribute(hConsole, color);
        char* loglevel = nullptr;

        switch (level)
        {
        case LogLevel::NONE:
        {
            printf("[%s] ", DateTime::GetTime());
            break;
        }
        case LogLevel::INPUT:
        {
            printf("[%s] [INPUT] ", DateTime::GetTime());
            break;
        }
        case LogLevel::RENDERING:
        {
            printf("[%s] [RENDERING] ", DateTime::GetTime());
            break;
        }
        case LogLevel::TEST:
        {
            printf("[%s] [TEST] ", DateTime::GetTime());
            break;
        }
        case LogLevel::JOBS:
        {
            printf("[%s] [JOBS] ", DateTime::GetTime());
            break;
        }
        case LogLevel::PROFILING:
        {
            printf("[%s] [PROFILING] ", DateTime::GetTime());
            break;
        }
        }

        va_list argList;
        va_start(argList, level);
        vsnprintf(s_buffer, 1024, msg, argList);
        va_end(argList);

        printf(s_buffer);
        printf("\n");

        Log(DebugInfoDesc(s_buffer, "", -1, LogLevel::NONE));
    }

    /// <summary>
    /// Log a formatted message.
    /// </summary>
    /// <param name="msg">The formatted message.</param>
    /// <param name="color">The color code for the message.</param>
    /// <param name="level">The log level.</param>
    /// <param name="">additional arguments</param>
    void Logger::Log(const String msg, const COLOR color = COLOR_WHITE, const LogLevel level = LogLevel::NONE, ...)
    {
        auto lock = ScopedSpinLock(logSpinLock);
        if (!LogLvlActive(level)) { return; }

        static char s_buffer[1024];

        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        char* loglevel = nullptr;

        switch (level)
        {
        case LogLevel::NONE:
        {
            printf("[%s] ", DateTime::GetTime());
            break;
        }
        case LogLevel::INPUT:
        {
            printf("[%s] [INPUT] ", DateTime::GetTime());
            break;
        }
        case LogLevel::RENDERING:
        {
            printf("[%s] [RENDERING] ", DateTime::GetTime());
            break;
        }
        case LogLevel::TEST:
        {
            printf("[%s] [TEST] ", DateTime::GetTime());
            break;
        }
        }

        va_list argList;
        va_start(argList, level);
        vsnprintf(s_buffer, 1024, msg.Value(), argList);
        va_end(argList);

        printf(s_buffer);
        printf("\n");

        Log(DebugInfoDesc(s_buffer, "", -1, LogLevel::NONE));
    }

    void Logger::Log(const DebugInfoDesc desc)
    {
        logCount[(int)desc.debugType]++;
        logBuffer.push_back(desc);
        CapDebugBuffer();
    }

    void Logger::ClearDebugBuffer()
    {
        ZeroMemory(&logCount[0], sizeof(logCount));
        logBuffer.clear();
    }

    void Logger::CapDebugBuffer()
    {
        if (logBuffer.size() > 999)
        {
            logCount[(int)logBuffer.at(0).debugType]--;
            logBuffer.erase(logBuffer.begin(), logBuffer.begin() + logBuffer.size() - 999);
        }
    }

    std::vector<Logger::DebugInfoDesc> Logger::logBuffer = std::vector<Logger::DebugInfoDesc>();
    size_t Logger::logCount[3] = {0,0,0};


    /// <summary>
    /// The spinlock used to synchronize the logging
    /// </summary>
    SpinLock Logger::logSpinLock = SpinLock();

    /// <summary>
    /// The console window handle.
    /// </summary>
    HANDLE Logger::hConsole = NULL;

    /// <summary>
    /// The bitmask to set the logging verbosity.
    /// </summary>
    DWORD Logger::logLvLFilterMask = 0b000110101;

    /*
    enum LogLevel
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
    */
}
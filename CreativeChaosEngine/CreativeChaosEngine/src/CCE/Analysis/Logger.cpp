#include "logger.h"
#include "../string/string.h"
#include "../manager/ProfilingManager.h"
#include "../multithreading/scoped-spinlock.h"
#include "debugInfoDesc.h"

namespace CCE
{
    HANDLE g_hConsole = nullptr;
    DWORD g_logLvLFilterMask = 0b001110101;

    std::vector<DebugInfoDesc> g_logBuffer = std::vector<DebugInfoDesc>();
    size_t g_logCount[3] = { 0, 0, 0 };

    SpinLock g_logConstructSpinlock{};
    SpinLock g_logSpinlock{};

    /// <summary>
    /// Initializes the console to log to.
    /// </summary>
    void InitializeLogger()
    {
        if (g_hConsole == nullptr)
        {
            g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTitleA("CCE Debug Console");
        }
    }

    /// <summary>
    /// Deinitializes the logging console.
    /// </summary>
    void DeinitializeLogger()
    {
        g_hConsole = nullptr;
    }

    
    bool LogLvlActive(const LogLevel msgLogLvl)
    {
        return (g_logLvLFilterMask & 1 << (int)msgLogLvl) != 0;
    }

    void SetLogLvlMaks(const DWORD mask)
    {
        g_logLvLFilterMask = mask;
    }

    /// <summary>
    /// Log a formatted message.
    /// </summary>
    /// <param name="msg">The formatted message.</param>
    /// <param name="color">The color code for the message.</param>
    /// <param name="level">The log level.</param>
    /// <param name="">Additional arguments.</param>
    void Log(const char* msg, const COLOR color = COLOR_WHITE, const LogLevel level = LogLevel::NONE, ...)
    {
        OPTICK_EVENT();

        if (!LogLvlActive(level)) { return; }
        ScopedSpinLock lock(g_logConstructSpinlock);

        char s_buffer[1024] = { 0 };
        ZeroMemory(&s_buffer, 1024);

        SetConsoleTextAttribute(g_hConsole, color);
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
    /// <param name="">Additional arguments.</param>
    void Log(const String msg, const COLOR color = COLOR_WHITE, const LogLevel level = LogLevel::NONE, ...)
    {
        OPTICK_EVENT();

        if (!LogLvlActive(level)) { return; }
        ScopedSpinLock lock(g_logConstructSpinlock);

        char s_buffer[1024] = {0};
        ZeroMemory(&s_buffer, 1024);

        SetConsoleTextAttribute(g_hConsole, color);
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

    void Log(const DebugInfoDesc desc)
    {
        OPTICK_EVENT();
        ScopedSpinLock lock(g_logSpinlock);

        g_logCount[(int)desc.debugType]++;
        g_logBuffer.push_back(desc);
        CapDebugBuffer();
    }

    void ClearDebugBuffer()
    {
        OPTICK_EVENT();
        ZeroMemory(&g_logCount[0], 3);
        g_logBuffer.clear();
    }

    void CapDebugBuffer()
    {
        OPTICK_EVENT();
        if (g_logBuffer.size() > 999)
        {
            g_logCount[(int)g_logBuffer.at(0).debugType]--;
            g_logBuffer.erase(g_logBuffer.begin(), g_logBuffer.begin() + g_logBuffer.size() - 999);
        }
    }   
}
#include "Logger.h"
#include "Time.h"


// TODO: Implement ability to filter Log lvl
namespace CCE
{
    /// <summary>
    /// Log a formatted message.
    /// </summary>
    /// <param name="msg">The formatted message.</param>
    /// <param name="color">The color code for the message.</param>
    /// <param name="level">The log level.</param>
    /// <param name="">additional arguments</param>
    void Logger::Log(const char* msg, COLOR color = COLOR_WHITE, LogLevel level = NONE, ...)
    {
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
        }

        va_list argList;
        va_start(argList, level);
        vsnprintf(s_buffer, 1024, msg, argList);
        va_end(argList);

        printf(s_buffer);
        printf("\n");
    }

    /// <summary>
    /// Log a formatted message.
    /// </summary>
    /// <param name="msg">The formatted message.</param>
    /// <param name="color">The color code for the message.</param>
    /// <param name="level">The log level.</param>
    /// <param name="">additional arguments</param>
    void Logger::Log(const String msg, COLOR color = COLOR_WHITE, LogLevel level = NONE, ...)
    {
        static char s_buffer[1024];

        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        char* loglevel;

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
    }

    /// <summary>
    /// The console window handle.
    /// </summary>
    HANDLE Logger::hConsole = NULL;
}
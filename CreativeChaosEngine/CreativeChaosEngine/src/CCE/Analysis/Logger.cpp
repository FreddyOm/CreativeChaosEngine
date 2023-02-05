#include "Logger.h"
#include "Time.h"

namespace CCE
{    
    void Logger::Log(const char* msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        printf("[%s] %s\n", DateTime::GetTime(), msg);
    }

    void Logger::Log(const String msg, COLOR color)
    {
        Logger::Log(msg.Value(), color);
    }

    void Logger::Log(const float msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        printf("[%s] %f\n", DateTime::GetTime(), msg);
    }

    void Logger::Log(const double msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        printf("[%s] %f\n", DateTime::GetTime(), msg);
    }

    void Logger::Log(const int msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        printf("[%s] %i\n", DateTime::GetTime(), msg);
    }

    void Logger::Log(const long msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        printf("[%s] %i\n", DateTime::GetTime(), msg);
    }

    HANDLE Logger::hConsole;
}
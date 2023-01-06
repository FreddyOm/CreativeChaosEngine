#include "Logger.h"
#include <iostream>
#include "Time.h"
#include <ctime>


namespace CCE
{    
    void Logger::Log(const char* msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        std::cout << "[" << DateTime::GetTime() << "] " << msg << std::endl;
    }

    void Logger::Log(const float msg, COLOR color)
    {
        if (hConsole == NULL)
        {
            hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        SetConsoleTextAttribute(hConsole, color);
        std::cout << "[" << DateTime::GetTime() << "] " << msg << std::endl;
    }

    HANDLE Logger::hConsole;
}
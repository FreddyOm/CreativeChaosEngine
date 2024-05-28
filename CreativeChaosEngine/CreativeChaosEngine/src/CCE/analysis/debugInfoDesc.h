#pragma once
#include <string>
#include "time.h"

enum class LogLevel;

namespace CCE
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
        LogLevel debugType = (LogLevel)0; // NONE
        std::string fileName = "";
        std::string msg = "";
    };
}

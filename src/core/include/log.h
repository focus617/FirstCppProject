#pragma once

#include <iostream>

namespace xuzy {

using std::cout;
using std::endl;

class Log
{
    public:
        enum Level { LevelError = 0, LevelWarning, LevelInfo, LevelDebug };

    private:
        Level m_LogLevel = LevelInfo;
    
    public:
        Log() = default;

        void setLevel(const Level level);
        void error(const char* message);
        void warn(const char* message);
        void info(const char* message);
        void debug(const char* message);
};



}
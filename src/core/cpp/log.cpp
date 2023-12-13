#include "log.h"

void xuzy::Log::setLevel(const Level level){
    this->m_LogLevel = level;
}

void xuzy::Log::error(const char* message){
    if(m_LogLevel >= LevelError)
        cout << "[ERROR]: " << message << endl;
}

void xuzy::Log::warn(const char* message){
    if(m_LogLevel >= LevelWarning)
        cout << "[WARN]: " << message << endl;
}

void xuzy::Log::info(const char* message){
    if(m_LogLevel >= LevelInfo)
        cout << "[INFO]: " << message << endl;
}

void xuzy::Log::debug(const char* message){
    if(m_LogLevel >= LevelDebug)
        cout << "[DEBUG]: " << message << endl;
}
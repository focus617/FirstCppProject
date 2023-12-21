#include "log.h"

xuzy::Log::Level xuzy::Log::m_LogLevel = Log::LevelInfo;
std::mutex xuzy::Log::m_LogMutex;
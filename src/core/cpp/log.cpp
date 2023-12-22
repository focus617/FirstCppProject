#include "log.h"

using namespace xuzy;

Log::Level Log::m_level {Log::Level::Info};
std::mutex Log::m_mutex;
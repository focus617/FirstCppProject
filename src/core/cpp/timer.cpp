#include "timer.h"

std::ostream &xuzy::operator<<(std::ostream &stream, const xuzy::Timer &timer)
{
    stream << timer.m_duration.count() * 1000.0f << "ms";
    return stream;
}
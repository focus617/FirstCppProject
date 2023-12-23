#pragma once

#include <iostream>
#include <chrono>

namespace xuzy
{
    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_start, m_end;
        std::chrono::duration<float> m_duration;

    public:
        Timer()
        {
            m_start = std::chrono::high_resolution_clock::now();
        }

        ~Timer()
        {
            m_end = std::chrono::high_resolution_clock::now();
            m_duration = m_end - m_start;

            std::cout << "Timer took " << m_duration.count() * 1000.0f << "ms" << std::endl;
        }
    };
}
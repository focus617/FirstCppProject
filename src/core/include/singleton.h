#pragma once

#include <iostream>
#include <mutex>

// #include "log.h"

class Singleton
{
private:
    static Singleton *m_one_and_only_instance;
    static int m_unique_id;
    static std::mutex m_mutex;

    Singleton(const int t_num)
    {
        m_unique_id = t_num;
    }

public:
    static Singleton *get_instance(const int t_num)
    {
        // thread safe
        std::scoped_lock lock(m_mutex);

        if (m_one_and_only_instance == nullptr)
        {
            std::cout << "creating a new instance" << std::endl;
            // lazy initialization
            m_one_and_only_instance = new Singleton(t_num);
        }
        std::cout << "returning instance with unique id: " << m_unique_id << std::endl;
        return m_one_and_only_instance;
    }

    Singleton(Singleton &t_other) = delete;
    void operator=(const Singleton &) = delete;

    int get_unique_id()
    {
        return this->m_unique_id;
    }

    void print_unique_id()
    {
        std::cout << "Instance's unique id: " << this->m_unique_id << std::endl;
    }
};

Singleton *Singleton::m_one_and_only_instance = nullptr;
int Singleton::m_unique_id = 0;
std::mutex Singleton::m_mutex;
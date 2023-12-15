#pragma once

#include <iostream>
#include <mutex>

class Singleton {
private:
    static Singleton* one_and_only_instance;
    static int unique_id;
    static std::mutex m_Mutex;

    Singleton(const int num){
        unique_id = num;
    }

public:
    static Singleton* get_instance(const int num) {
        // thread safe
        std::scoped_lock lock(m_Mutex);

        if(one_and_only_instance == nullptr) {
            std::cout << "creating a new instance" << std::endl;
            // lazy initialization
            one_and_only_instance = new Singleton(num);
        } 
        std::cout << "returning instance with unique id: " << unique_id << std::endl;
        return one_and_only_instance;
    }

    Singleton(Singleton &other) = delete;
    void operator=(const Singleton &) = delete;

    void PrintUniqueID() {
        std::cout << "Instance's unique id: " << this->unique_id << std::endl;
    }

};

Singleton* Singleton::one_and_only_instance = nullptr;
int Singleton::unique_id = 0;
std::mutex Singleton::m_Mutex;
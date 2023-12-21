#include <iostream>
#include <thread>

#include "log.h"

using xuzy::Log;

static bool s_finished = false;

void do_work()
{
    using namespace std::literals::chrono_literals;
    
    std::cout << "Worker start with thread id: " << std::this_thread::get_id() << std::endl;

    while (!s_finished)
    {
        LOG_INFO("Worker thread [id: %d] is working ...", std::this_thread::get_id());
        std::this_thread::sleep_for(1s);
    }
    LOG_INFO("Worker thread [id: %d] is finished.", std::this_thread::get_id());
     
}

void thread_trial()
{
    std::thread worker(do_work);

    std::cin.get();
    s_finished = true;

    worker.join();

    LOG_INFO("Main thread [id: %d] is finished.", std::this_thread::get_id());

    return;
}
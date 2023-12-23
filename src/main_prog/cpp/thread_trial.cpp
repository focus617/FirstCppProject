#include <iostream>
#include <thread>

#include "log.h"

using xuzy::Log;

static bool s_finished = false;

void do_work()
{
    using namespace std::literals::chrono_literals;

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Worker thread [id: " << std::this_thread::get_id() << "] start." << std::endl;

    while (!s_finished)
    {
        auto wakeup = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = wakeup - start;
        start = wakeup;

        LOG_INFO("Worker thread [id: %d] wake up after %fs ...",
                 std::this_thread::get_id(), duration.count());

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
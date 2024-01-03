#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <span>
#include <chrono>

#include "logger.h"
#include "mastercontroller.h"

namespace xuzy
{
    using namespace std::chrono_literals;
    class Worker
    {
    private:
        Master_Controller *p_master;
        std::jthread m_thread;
        std::condition_variable m_cv;
        std::mutex m_mtx;

        // shared memory
        std::span<int> m_input;
        int *p_output = nullptr;
        bool m_dying = false;

        void Run_();
        void process_dataset(std::span<int> t_input, int &t_output);

    public:
        Worker(Master_Controller *t_master);
        ~Worker();

        void set_job(std::span<int> t_input, int *t_output);
        void kill();
    };

    Worker::Worker(Master_Controller *t_master) : p_master{t_master}, m_thread{&Worker::Run_, this}
    {
        LOG_INFO("Worker [thread id: %d] is created.", m_thread.get_id());
    }

    Worker::~Worker()
    {
        LOG_INFO("Worker [thread id: %d] died.", m_thread.get_id());
    }

    void Worker::Run_()
    {
        std::unique_lock<std::mutex> locker{m_mtx};

        while (true)
        {
            m_cv.wait(locker, [this]()
                      { return p_output != nullptr || m_dying; });

            LOG_INFO("Worker [thread id: %d] wake up...", std::this_thread::get_id());

            if (m_dying)
            {
                break;
            }

            process_dataset(m_input, *p_output);
            p_output = nullptr;
            m_input = {};
            p_master->signal_done(); // notify master that task has done
        }
    }

    void Worker::process_dataset(std::span<int> t_input, int &t_output)
    {
        t_output = t_input.size();
        
        LOG_INFO("Worker [thread id: %d] process dataset", std::this_thread::get_id());
        std::this_thread::sleep_for(1s);
    }

    void Worker::set_job(std::span<int> t_input, int *t_output)
    {
        {
            std::lock_guard locker{m_mtx};
            m_input = t_input;
            p_output = t_output;
        }
        m_cv.notify_one();
    }

    void Worker::kill()
    {
        {
            std::lock_guard locker{m_mtx};
            m_dying = true;
        }
        m_cv.notify_one();
    }
}
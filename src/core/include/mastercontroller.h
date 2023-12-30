#pragma once

#include <thread>
#include <mutex>
#include <chrono>

#include "log.h"
namespace xuzy
{
    using namespace std::chrono_literals;

    class Master_Controller
    {
    private:
        std::condition_variable m_cv;
        std::mutex m_mtx;
        std::unique_lock<std::mutex> m_lk;
        unsigned int m_worker_count;

        // shared memory
        unsigned int m_done_count = 0;

    public:
        Master_Controller(unsigned int t_worker_count);
        ~Master_Controller();

        void signal_done();
        void wait_for_all_done();
    };

    Master_Controller::Master_Controller(unsigned int t_worker_count)
        : m_lk{m_mtx}, m_worker_count{t_worker_count}
    {
    }

    Master_Controller::~Master_Controller()
    {
    }

    void Master_Controller::signal_done()
    {
        {
            std::lock_guard lk{m_mtx};
            ++m_done_count;
        }
        if (m_done_count == m_worker_count)
        {
            m_cv.notify_one(); // notify master that all work done
            LOG_INFO("Master Controller [thread id: %d]: receive signal_done.", std::this_thread::get_id());
        }
    }

    void Master_Controller::wait_for_all_done()
    {
        LOG_INFO("Master Controller [thread id: %d]: wait for signal_done.", std::this_thread::get_id());
        m_cv.wait(m_lk, [this]
                  { return m_done_count == m_worker_count; });
        m_done_count = 0;
    }
}
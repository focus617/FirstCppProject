#pragma once

#include <unistd.h>

#include <cassert>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "core/visibility_control.hpp"

#ifndef CPPHTTPLIB_THREAD_POOL_COUNT
#  define CPPHTTPLIB_THREAD_POOL_COUNT                            \
    ((std::max)(8u, std::thread::hardware_concurrency() > 0       \
                        ? std::thread::hardware_concurrency() - 1 \
                        : 0))
#endif

namespace xuzy {

/**
 * @brief TaskQueue derived from cpp-httplib
 * This is an interface class for ThreadPool
 */
class XUZY_API TaskQueue {
 public:
  XUZY_API TaskQueue() = default;
  XUZY_API virtual ~TaskQueue() = default;

  /**
   * @brief Put new coming task into queue
   * @param fn New task
   * @return true if task is loaded into queue, otherwise false
   */
  XUZY_API
  virtual bool enqueue(std::function<void()> fn) = 0;

  /**
   * @brief Shutdown Taskqueue...
   */
  XUZY_API
  virtual void shutdown() = 0;

  /**
   * @brief Callback when Queue empty
   */
  XUZY_API
  virtual void on_idle() {}
};

/**
 * @brief Thread Pool example derived from cpp-httplib
 */
class XUZY_API ThreadPool : public TaskQueue {
 public:
  /**
   * @brief Constructor for ThreadPool
   * @param n Maxium number of Worker in ThreadPool
   * @param mqr Maxium size of Job queue, no limitation when mqr equals to zero
   */
  XUZY_API
  explicit ThreadPool(size_t n, size_t mqr = 0)
      : shutdown_(false), max_queued_requests_(mqr) {
    // Create workers
    while (n) {
      threads_.emplace_back(worker(*this));
      n--;
    }
  }

  ThreadPool(const ThreadPool&) = delete;

  ~ThreadPool() override {
    if (!shutdown_) shutdown();
  };

  /**
   * @brief Put new coming job into queue
   * @param fn New job
   * @return true if job was loaded into queue, otherwise false
   */
  XUZY_API
  bool enqueue(std::function<void()> fn) override {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      // Reject if there is size limitation for queue and
      // queue has no more space
      if (max_queued_requests_ > 0 && jobs_.size() >= max_queued_requests_) {
        return false;
      }
      jobs_.push_back(std::move(fn));
    }

    cond_.notify_one();
    return true;
  }

  void shutdown() override {
    // Stop all worker threads...
    {
      std::unique_lock<std::mutex> lock(mutex_);
      shutdown_ = true;
    }

    cond_.notify_all();

    // Join...
    for (auto& t : threads_) {
      t.join();
    }
  }

 private:
  struct worker {
    explicit worker(ThreadPool& pool) : pool_(pool) {}

    void operator()() {
      for (;;) {
        std::function<void()> fn;
        {
          std::unique_lock<std::mutex> lock(pool_.mutex_);

          pool_.cond_.wait(
              lock, [&] { return !pool_.jobs_.empty() || pool_.shutdown_; });

          if (pool_.shutdown_ && pool_.jobs_.empty()) {
            break;
          }

          fn = std::move(pool_.jobs_.front());
          pool_.jobs_.pop_front();
        }

        assert(true == static_cast<bool>(fn));
        fn();
      }
    }

    ThreadPool& pool_;
  };
  friend struct worker;

  std::vector<std::thread> threads_;
  std::list<std::function<void()>> jobs_;

  bool shutdown_;
  size_t max_queued_requests_ = 0;

  std::condition_variable cond_;
  std::mutex mutex_;
};

}  // namespace xuzy

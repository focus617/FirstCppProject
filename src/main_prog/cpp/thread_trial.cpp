#include <thread>
#include <vector>

#include "helper/timer.hpp"
#include "logger.h"

using xuzy::Logger;

static bool s_finished = false;

struct Shared_Data {
  int counter{0};
  std::mutex mtx;
};

static Shared_Data data;

void do_work(int t_int) {
  using namespace std::literals::chrono_literals;

  auto start = std::chrono::high_resolution_clock::now();
  LOG_INFO("Worker-%d thread [id: %d] start.", t_int,
           std::this_thread::get_id());

  while (!s_finished) {
    std::this_thread::sleep_for(1s);
    auto wakeup = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = wakeup - start;
    start = wakeup;

    LOG_INFO("Worker-%d thread [id: %d] wake up after %fs ...", t_int,
             std::this_thread::get_id(), duration.count());

    std::lock_guard lg{data.mtx};
    data.counter++;
    LOG_INFO("Worker-%d thread data.counter after increacement: %d", t_int,
             data.counter);
  }
  LOG_INFO("Worker-%d thread [id: %d] is finished.", t_int,
           std::this_thread::get_id());
}

void thread_trial() {
  unsigned int t_thread_num = std::thread::hardware_concurrency();
  LOG_INFO("CPU core: %d", t_thread_num);

  std::vector<std::thread> threads;
  xuzy::Timer timer;

  timer.mark();

  for (size_t i = 0; i < t_thread_num; ++i) {
    threads.emplace_back(std::thread(do_work, i));
  }

  std::cin.get();
  s_finished = true;

  for (auto& thread : threads) {
    thread.join();
  }

  LOG_INFO("Main thread [id: %d] is finished.", std::this_thread::get_id());

  timer.peek();
  LOG_INFO("Threads runs %fms.", timer.count_in_ms());

  return;
}
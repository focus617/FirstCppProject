#include <algorithm>
#include <array>
#include <random>
#include <ranges>
#include <thread>
#include <vector>

#include "tools/time/timer.hpp"
#include "logger.h"
#include "worker.hpp"

using xuzy::Logger;

void thread_active_object() {
  std::minstd_rand rne;
  std::array<int, 100> dataset;
  std::ranges::generate(dataset, rne);

  unsigned int t_thread_num = std::thread::hardware_concurrency();
  LOG_INFO("CPU core: %d", t_thread_num);

  xuzy::Master_Controller mctrl{t_thread_num};

  std::vector<std::unique_ptr<xuzy::Worker>> worker_ptrs;

  for (size_t i = 0; i < t_thread_num; i++) {
    worker_ptrs.push_back(std::make_unique<xuzy::Worker>(&mctrl));
  }

  std::array<int, 12> result;

  xuzy::Timer timer;
  timer.mark();

  for (size_t i = 0; i < t_thread_num; i++) {
    worker_ptrs[i]->set_job(dataset, &result[i]);
  }
  mctrl.wait_for_all_done();

  timer.peek();
  LOG_INFO("Processing took %fms.", timer.count_in_ms());

  for (auto& worker_ptr : worker_ptrs) {
    worker_ptr->kill();
  }
  worker_ptrs.clear();
}

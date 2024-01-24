#pragma once

#include <glog/logging.h>
#include <chrono>
#include <iomanip>
#include <iostream>

#include "visibility_control.hpp"

namespace xuzy {
class XUZY_API Timer {
  friend std::ostream& operator<<(std::ostream& stream, const Timer& timer);

 private:
  std::chrono::time_point<std::chrono::system_clock> m_start, m_end;
  std::chrono::duration<float> m_duration;

 public:
  Timer() { m_start = std::chrono::high_resolution_clock::now(); }

  ~Timer() {
    peek();

    LOG(INFO) << "Timer exist for " << count_in_ms() << "ms totally.";
  }

  void mark() { m_start = std::chrono::high_resolution_clock::now(); }

  void peek() {
    m_end = std::chrono::high_resolution_clock::now();
    m_duration = m_end - m_start;
  }

  float count_in_ms() { return m_duration.count() * 1000.0f; }

  static const std::string get_current_timestamp();
};

std::ostream& operator<<(std::ostream& stream, const Timer& timer);

}  // namespace xuzy
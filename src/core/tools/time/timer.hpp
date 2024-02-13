#pragma once

#include <glog/logging.h>

#include <chrono>
#include <iomanip>
#include <iostream>

#include "core/visibility_control.hpp"

namespace xuzy {

// Integer types of known sizes.
using TimeInMillis = int64_t;  // Represents time in milliseconds.

/**
 * @brief A helper class for measuring elapsed times.
 */
class XUZY_API Timer {
 public:
  XUZY_API Timer() { m_start = std::chrono::high_resolution_clock::now(); }

  /**
   * @brief Return the time elapsed since clock creation
   */
  XUZY_API ~Timer() {
    peek();

    LOG(INFO) << "Timer exist for " << count_in_ms() << "ms totally.";
  }

  /**
   * @brief Reset the timer and restart
   */
  XUZY_API
  void mark() { m_start = std::chrono::high_resolution_clock::now(); }

  /**
   * @brief Measure time elapsed since timer was created or marked
   */
  XUZY_API
  void peek() {
    m_end = std::chrono::high_resolution_clock::now();
    m_duration = m_end - m_start;
  }

  /**
   * @brief Return elapse time in millisecond
   */
  float count_in_ms() { return m_duration.count() * 1000.0f; }

  static const std::string get_current_timestamp();

  friend std::ostream& operator<<(std::ostream& stream, const Timer& timer);

 private:
  std::chrono::time_point<std::chrono::system_clock> m_start, m_end;
  std::chrono::duration<float> m_duration;
};

std::ostream& operator<<(std::ostream& stream, const Timer& timer);

}  // namespace xuzy
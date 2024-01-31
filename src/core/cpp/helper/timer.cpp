#include "pch.h"

#include "helper/timer.hpp"

std::ostream& xuzy::operator<<(std::ostream& stream, const xuzy::Timer& timer) {
  stream << timer.m_duration.count() * 1000.0f << "ms";
  return stream;
}

const std::string xuzy::Timer::get_current_timestamp() {
  // Get the current time
  auto now = std::chrono::system_clock::now();

  // Convert to a time_point
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  // Format as a string
  std::tm* timestamp = std::localtime(&now_c);

  // Buffering the timestamp in the string stream.
  std::stringstream ss;
  ss << std::put_time(timestamp, "%Y-%m-%d %H:%M:%S");

  return ss.str();
}
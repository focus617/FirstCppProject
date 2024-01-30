#pragma once

#include <cstdint>

#include "core/visibility_control.hpp"

namespace xuzy {

/**
 * @brief A simple Linear Congruential Generator for generating random numbers
 with a uniform distribution. Unlike rand() and srand(), it doesn't use global
 state (and therefore can't interfere with user code). Unlike rand_r(), it's
 portable. An LCG isn't very random, but it's good enough for our purposes.
*/
class XUZY_API Random {
 public:
  static const uint32_t kMaxRange = 1u << 31;

  XUZY_API explicit Random(uint32_t seed) : state_(seed) {}

  XUZY_API void Reseed(uint32_t seed) { state_ = seed; }

  /**
   * @brief Generates a random number from [0, range), using a Linear
   * Congruential Generator (LCG). Crashes if 'range' is 0 or greater than
   * kMaxRange.
   */
  XUZY_API uint32_t Generate(uint32_t range);

 private:
  uint32_t state_;
  Random(const Random&) = delete;
  Random& operator=(const Random&) = delete;
};

}  // namespace xuzy
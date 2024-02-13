#include "pch.h"

#include "random.hpp"

namespace xuzy {

const uint32_t Random::kMaxRange;

// Generates a random number from [0, range), using a Linear
// Congruential Generator (LCG).  Crashes if 'range' is 0 or greater
// than kMaxRange.
uint32_t Random::Generate(uint32_t range) {
  XUZY_CHECK_(range > 0) << "Cannot generate a number in the range [0, 0).";

  XUZY_CHECK_(range <= kMaxRange)
      << "Generation of a number in [0, " << range << ") was requested, "
      << "but this can only generate numbers in [0, " << kMaxRange << ").";

  // These constants are the same as are used in glibc's rand(3).
  // Use wider types than necessary to prevent unsigned overflow diagnostics.
  state_ = static_cast<uint32_t>(1103515245ULL * state_ + 12345U) % kMaxRange;

  // Converting via modulus introduces a bit of downward bias, but
  // it's simple, and a linear congruential generator isn't too good
  // to begin with.
  return state_ % range;
}

}  // namespace xuzy
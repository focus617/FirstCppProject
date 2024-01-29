#include "resource/random.hpp"

#include <gtest/gtest.h>

// Tests the Random class.

TEST(RandomTest, GeneratesDeathOnInvalidRange) {
  xuzy::Random random(42);
  EXPECT_DEATH_IF_SUPPORTED(random.Generate(0),
                            "Cannot generate a number in the range \\[0, 0\\)");
  EXPECT_DEATH_IF_SUPPORTED(
      random.Generate(xuzy::Random::kMaxRange + 1),
      "Generation of a number in \\[0, 2147483649\\) was requested, "
      "but this can only generate numbers in \\[0, 2147483648\\)");
}

TEST(RandomTest, GeneratesNumbersWithinRange) {
  constexpr uint32_t kRange = 10000;
  xuzy::Random random(12345);
  for (int i = 0; i < 10; i++) {
    EXPECT_LT(random.Generate(kRange), kRange) << " for iteration " << i;
  }

  xuzy::Random random2(xuzy::Random::kMaxRange);
  for (int i = 0; i < 10; i++) {
    EXPECT_LT(random2.Generate(kRange), kRange) << " for iteration " << i;
  }
}

TEST(RandomTest, RepeatsWhenReseeded) {
  constexpr int kSeed = 123;
  constexpr int kArraySize = 10;
  constexpr uint32_t kRange = 10000;
  uint32_t values[kArraySize];

  xuzy::Random random(kSeed);
  for (int i = 0; i < kArraySize; i++) {
    values[i] = random.Generate(kRange);
  }

  random.Reseed(kSeed);
  for (int i = 0; i < kArraySize; i++) {
    EXPECT_EQ(values[i], random.Generate(kRange)) << " for iteration " << i;
  }
}
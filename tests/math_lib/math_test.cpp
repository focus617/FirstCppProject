#include <gtest/gtest.h>
#include "xuzy_math.h"

// Demonstrate some basic assertions.
TEST(MathTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

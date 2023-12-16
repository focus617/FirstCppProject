#include <gtest/gtest.h>
#include "xuzy_math.h"

// Demonstrate some basic assertions.
TEST(MathTest, BasicAssertions) {
  // Expect two strings not to be equal.
  int result = xuzy::add(7, 12);
  // Expect equality.
  EXPECT_EQ(result, 19);
}

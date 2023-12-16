#include <gtest/gtest.h>
#include "xuzy_math.h"

TEST(MathTest, BasicAddtions) {
  int result = xuzy::add(7, 12);
  // Expect equality.
  EXPECT_EQ(result, 19);
}

TEST(MathTest, AssertNoEqual) {
  // Expect equality.
  ASSERT_EQ(xuzy::add(7, 12), 18) << "Oh no, a mistake!";
}

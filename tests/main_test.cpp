#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  std::cout << "Init Testing" << std::endl;
  return RUN_ALL_TESTS();
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

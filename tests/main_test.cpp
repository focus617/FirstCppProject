#include <gtest/gtest.h>
#include <gmock/gmock.h>

// int main(int argc, char **argv){
//   testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

using ::testing::Eq;

TEST(HelloTest, TrailAssertThat) {
  // ASSERT_THAT from gmock.
  ASSERT_THAT(12+7, Eq(19));
}

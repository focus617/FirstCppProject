#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glog/logging.h>

int main(int argc, char **argv)
{
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv[0]);
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

using ::testing::Eq;

TEST(HelloTest, TrailAssertThat)
{
  // ASSERT_THAT from gmock.
  ASSERT_THAT(12 + 7, Eq(19));
}

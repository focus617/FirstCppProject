#include "tools/utils/container_utility.hpp"

#include <gtest/gtest.h>
#include "tools/maths/random.hpp"

using namespace xuzy;

// Tests STL container utilities

// Tests CountIf()

static bool IsPositive(int n) { return n > 0; }

TEST(ContainerUtilityTest, CountIf) {
  std::vector<int> v;
  EXPECT_EQ(0, CountIf(v, IsPositive));  // Works for an empty container.

  v.push_back(-1);
  v.push_back(0);
  EXPECT_EQ(0, CountIf(v, IsPositive));  // Works when no value satisfies.

  v.push_back(2);
  v.push_back(-10);
  v.push_back(10);
  EXPECT_EQ(2, CountIf(v, IsPositive));
}

// Tests ForEach()

static int g_sum = 0;
static void Accumulate(int n) { g_sum += n; }

TEST(ContainerUtilityTest, ForEach) {
  std::vector<int> v;
  g_sum = 0;
  ForEach(v, Accumulate);
  EXPECT_EQ(0, g_sum);  // Works for an empty container;

  g_sum = 0;
  v.push_back(1);
  ForEach(v, Accumulate);
  EXPECT_EQ(1, g_sum);  // Works for a container with one element.

  g_sum = 0;
  v.push_back(20);
  v.push_back(300);
  ForEach(v, Accumulate);
  EXPECT_EQ(321, g_sum);
}

// Tests GetElementOr()

TEST(ContainerUtilityTest, GetElementOr) {
  std::vector<char> a;
  EXPECT_EQ('x', GetElementOr(a, 0, 'x'));

  a.push_back('a');
  a.push_back('b');
  EXPECT_EQ('a', GetElementOr(a, 0, 'x'));
  EXPECT_EQ('b', GetElementOr(a, 1, 'x'));
  EXPECT_EQ('x', GetElementOr(a, -2, 'x'));
  EXPECT_EQ('x', GetElementOr(a, 2, 'x'));
}

#ifdef NDEBUG
TEST(ContainerUtilityDeathTest, ShuffleRange) {
  std::vector<int> a;
  a.push_back(0);
  a.push_back(1);
  a.push_back(2);
  xuzy::Random random(1);

  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, -1, 1, &a),
      "Invalid shuffle range start -1: must be in range \\[0, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 4, 4, &a),
      "Invalid shuffle range start 4: must be in range \\[0, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 3, 2, &a),
      "Invalid shuffle range finish 2: must be in range \\[3, 3\\]");
  EXPECT_DEATH_IF_SUPPORTED(
      ShuffleRange(&random, 3, 4, &a),
      "Invalid shuffle range finish 4: must be in range \\[3, 3\\]");
}
#endif
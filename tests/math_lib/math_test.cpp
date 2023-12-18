#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include <iostream>
#include <tuple>

#include "xuzy_math.h"

class Math_Test_With_Param: public testing::TestWithParam<std::tuple<int, int, int>>
{
  // You can implement all the usual fixture class member here.
  // To access the test parameter, call GetParam() from class TestWithParam<T>.
};

TEST_P(Math_Test_With_Param, add_test_equal) {
  // access the test parameter
  auto [a, b, c] = GetParam();

  // Expect equality.
  EXPECT_EQ(xuzy::add(a, b), c) << "Oh no, a mistake!";
}

INSTANTIATE_TEST_SUITE_P(PositiveNumbers,
                          Math_Test_With_Param,
                          testing::Values(
                            std::tuple{1, 2, 3},
                            std::tuple{2, 5, 7}
                          )
                        );

INSTANTIATE_TEST_SUITE_P(NegativeNumbers,
                          Math_Test_With_Param,
                          testing::Values(
                            std::tuple{-1, -2, -3},
                            std::tuple{-2, 5, 3}
                          )
                        );

#include <gtest/gtest.h>
#include <iostream>
#include "logger.h"

class Log_Test_Fixture : public testing::Test
{
private:
  /* data */
public:
  Log_Test_Fixture(/* args */){}
  ~Log_Test_Fixture(){}

  void SetUp();
  void TearDown();
};

void Log_Test_Fixture::SetUp(/* args */)
{
}

void Log_Test_Fixture::TearDown()
{
}


TEST_F(Log_Test_Fixture, BasicAddtions) {
  // Expect equality.
  EXPECT_EQ(12+7, 19);
}

TEST_F(Log_Test_Fixture, AssertNoEqual) {
  // Expect not equality.
  ASSERT_NE(12+7, 18) << "Oh no, a mistake!";
}

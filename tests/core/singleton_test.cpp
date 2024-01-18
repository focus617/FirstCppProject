#include "singleton.h"

#include <gtest/gtest.h>

#include <iostream>

class Singleton_Test_Fixture : public testing::Test {
 private:
  /* data */
 public:
  Singleton_Test_Fixture(/* args */) {}
  ~Singleton_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

void Singleton_Test_Fixture::SetUp(/* args */) {}

void Singleton_Test_Fixture::TearDown() {}

TEST_F(Singleton_Test_Fixture, same_operation_result_on_two_instances) {
  const int value = 5;

  Singleton one = Singleton::Instance();
  // Expect equality.
  EXPECT_EQ(0, one.get_unique_id());

  one.set_unique_id(value);
  // Expect equality.
  EXPECT_EQ(value, one.get_unique_id());

  Singleton another = Singleton::Instance();
  // Expect equality.
  EXPECT_EQ(value, another.get_unique_id());
  EXPECT_EQ(one.get_unique_id(), another.get_unique_id());

  another.set_unique_id(value+55);
  // Expect equality.
  EXPECT_EQ(value+55, one.get_unique_id());
  EXPECT_EQ(one.get_unique_id(), another.get_unique_id());
}

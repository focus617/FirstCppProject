#include <gtest/gtest.h>
#include <iostream>

#include "singleton.h"

class Singleton_Test_Fixture : public testing::Test
{
private:
  /* data */
public:
  Singleton *p_singleton;

  Singleton_Test_Fixture(/* args */){}
  ~Singleton_Test_Fixture(){}

  void SetUp();
  void TearDown();
};

void Singleton_Test_Fixture::SetUp(/* args */)
{
    p_singleton = Singleton::get_instance(1);
}

void Singleton_Test_Fixture::TearDown()
{
}


TEST_F(Singleton_Test_Fixture, SameIntanceAfterGetAnother) {
  Singleton *p_another = Singleton::get_instance(5);
  // Expect equality.
  EXPECT_EQ(p_another, p_singleton);
}

TEST_F(Singleton_Test_Fixture, SameUniqueIdAfterGetAnother) {
  Singleton *p_another = Singleton::get_instance(5);
  // Expect equality.
  EXPECT_EQ(p_another->get_unique_id(), p_singleton->get_unique_id());
}



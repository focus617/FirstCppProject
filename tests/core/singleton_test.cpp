#include <gtest/gtest.h>

#include <iostream>

#include "resource/singleton.hpp"

TEST(Singleton_Test, same_operation_result_on_two_instances) {
  std::string empty("");
  std::string one("one");
  std::string change("change");

  Singleton* thisone = Singleton::GetInstance();
  // Expect equality.
  EXPECT_EQ(empty, thisone->get_unique_name());

  thisone->set_unique_name(one);
  // Expect equality.
  EXPECT_EQ(one, thisone->get_unique_name());

  Singleton* another = Singleton::GetInstance();
  // Expect equality.
  EXPECT_EQ(one, another->get_unique_name());
  EXPECT_EQ(thisone->get_unique_name(), another->get_unique_name());

  another->set_unique_name(change);
  // Expect equality.
  EXPECT_EQ(change, thisone->get_unique_name());
  EXPECT_EQ(thisone->get_unique_name(), another->get_unique_name());
}

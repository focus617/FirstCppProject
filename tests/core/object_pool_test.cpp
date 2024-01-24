#include "resource/object_pool.hpp"

#include <gtest/gtest.h>

using namespace xuzy;

class ObjectPool_Test_Fixture : public testing::Test {
 private:
  /* data */
 public:
  Resource* one;
  Resource* two;

  ObjectPool_Test_Fixture(/* args */) {}
  ~ObjectPool_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

void ObjectPool_Test_Fixture::SetUp(/* args */) {}

void ObjectPool_Test_Fixture::TearDown() {}

TEST_F(ObjectPool_Test_Fixture, basic) {
  ObjectPool pool = ObjectPool::Instance();

  /* Resources will be created. */
  one = pool.getResource();
  one->setValue(10);
  EXPECT_EQ(10, one->getValue());

  /* Resources will be reused.
   * Check that the value of resources should be reset back to zero.
   */
  pool.returnResource(one);
  EXPECT_NE(nullptr, one);
  EXPECT_EQ(0, one->getValue());
}

TEST_F(ObjectPool_Test_Fixture, request_two_resources) {
  ObjectPool pool = ObjectPool::Instance();

  /* Resources will be created. */
  one = pool.getResource();
  one->setValue(10);
  EXPECT_EQ(10, one->getValue());

  two = pool.getResource();
  two->setValue(20);
  EXPECT_EQ(20, two->getValue());

  pool.returnResource(one);
  pool.returnResource(two);
}

TEST_F(ObjectPool_Test_Fixture, resource_is_reused) {
  ObjectPool pool = ObjectPool::Instance();

  one = pool.getResource();
  pool.returnResource(one);

  two = pool.getResource();

  /* Resources will be reused. */
  EXPECT_EQ(one, two);

  pool.returnResource(two);
}
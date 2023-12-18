#include <gtest/gtest.h>
#include <iostream>
#include <memory>

#include "dummy.h"
#include "mock_foo.h"

class Dummy_Test_Fixture : public testing::Test
{
public:
  std::unique_ptr<Dummy> m_tested_dummy;

  MockFoo m_mock_foo;

  Dummy_Test_Fixture()
  {
    m_tested_dummy = std::make_unique<Dummy>(m_mock_foo);
  }
  ~Dummy_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

void Dummy_Test_Fixture::SetUp()
{
}

void Dummy_Test_Fixture::TearDown()
{
}

// 测试函数的返回值
TEST_F(Dummy_Test_Fixture, dummy_int)
{
  std::string str{"Hello World."};

  EXPECT_CALL(m_mock_foo, foo_int(str)).Times(1).WillOnce(testing::Return(str.size()));
  int result = m_tested_dummy->dummy_int(str);

  ASSERT_EQ(result, str.size());
}

// 测试函数的引用参数被正确修改
TEST_F(Dummy_Test_Fixture, dummy_str)
{
  std::string input_str{"Hello World."};
  std::string output_str{"ChangedString"};

  // testing::_表示忽略函数的参数，不需要检查
  EXPECT_CALL(m_mock_foo, foo_str(testing::_)).WillOnce(testing::SetArgReferee<0>(output_str));
  m_tested_dummy->dummy_str(input_str);

  ASSERT_EQ(input_str, output_str);
}

ACTION(throwError)
{
  std::cout << "Mock to throw exception" << std::endl;
  throw std::runtime_error("Throw Error Test");
}

void invoke_function()
{
  std::cout << "Mock function" << std::endl;
  throw std::runtime_error("Throw Error Test");
}

// 测试函数调用将丢出异常
TEST_F(Dummy_Test_Fixture, dummy_throw)
{
  // mock throw exception
  EXPECT_CALL(m_mock_foo, foo_throw()).WillOnce(throwError());

  // mock invoke function
  // EXPECT_CALL(m_mock_foo, foo_throw()).WillOnce(testing::Invoke(invoke_function));

  ASSERT_THROW(m_tested_dummy->dummy_throw(), std::runtime_error);
}

// 测试函数调用回调函数
TEST_F(Dummy_Test_Fixture, dummy_callback)
{
  // mock callback function
  testing::MockFunction<void(void)> testFunc;

  EXPECT_CALL(m_mock_foo, foo_callback_method(testing::_)).Times(1).WillOnce(testing::InvokeArgument<0>());
  EXPECT_CALL(testFunc, Call());

  m_tested_dummy->dummy_callback(testFunc.AsStdFunction());
}

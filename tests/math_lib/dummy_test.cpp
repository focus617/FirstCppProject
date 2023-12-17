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
  ~Dummy_Test_Fixture(){}

  void SetUp();
  void TearDown();
};

void Dummy_Test_Fixture::SetUp()
{    
}

void Dummy_Test_Fixture::TearDown()
{    
}

TEST_F(Dummy_Test_Fixture, dummy_int)
{
    std::string str {"Hello World."};

    EXPECT_CALL(m_mock_foo, foo_int(str)).Times(1).WillOnce(testing::Return(str.size()));
    int result = m_tested_dummy->dummy_int(str);

    ASSERT_EQ(result, str.size());
}


TEST_F(Dummy_Test_Fixture, dummy_str)
{
    std::string input_str {"Hello World."};
    std::string output_str {"ChangedString"};

    EXPECT_CALL(m_mock_foo, foo_str(input_str)).WillOnce(testing::SetArgReferee<0>(output_str));
    m_tested_dummy->dummy_str(input_str);

    ASSERT_EQ(input_str, output_str);
}
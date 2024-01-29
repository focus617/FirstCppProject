#include "resource/event.hpp"

#include <gtest/gtest.h>

static int checker = -1;

class Button {
 public:
  Button() {}

  virtual ~Button() {}

 public:
  // EventHandler:  void(*func)(int& result)
  xuzy::Event<void(int&)> OnClick;
};

void Click(int& result) {
  checker = 100;     // modify global static variable
  result = checker;  // set local variable
  std::cout << "Button Click(result=" << result << ")" << std::endl;
}

class Example {
 public:
  void Click(int& result) {
    checker = 200;
    result = checker;
    std::cout << "Example Click(result=" << result << ")" << std::endl;
  }
};

class Event_Test_Fixture : public testing::Test {
 public:
  Button button;

  Event_Test_Fixture() {}
  ~Event_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

void Event_Test_Fixture::SetUp() {}

void Event_Test_Fixture::TearDown() {}

TEST_F(Event_Test_Fixture, event_on_static_func) {
  // Given
  checker = 0;
  int r;

  // When: 静态函数做委托函数
  button.OnClick += Click;

  // Then
  EXPECT_EQ(0, checker);
  button.OnClick(r);
  EXPECT_EQ(100, checker);
  EXPECT_EQ(100, r);
}

TEST_F(Event_Test_Fixture, event_on_class_member_func) {
  checker = 0;
  int r;

  // When: 成员函数做委托函数
  Example example;
  button.OnClick += std::bind(&Example::Click, example, std::placeholders::_1);

  // Then
  EXPECT_EQ(0, checker);
  button.OnClick(r);
  EXPECT_EQ(200, checker);
  EXPECT_EQ(200, r);
}

TEST_F(Event_Test_Fixture, event_on_lambda_func) {
  checker = 0;
  int r;

  // When: 匿名函数做委托函数
  button.OnClick += [](int& result) {
    checker = 300;
    result = checker;
    std::cout << "Lambda Click(result=" << result << ")" << std::endl;
  };

  // Then
  EXPECT_EQ(0, checker);
  button.OnClick(r);
  EXPECT_EQ(300, checker);
  EXPECT_EQ(300, r);
}

TEST_F(Event_Test_Fixture, handler_count) {
  // Given

  // When: 静态函数做委托函数
  button.OnClick += Click;
  // Then
  EXPECT_EQ(1, button.OnClick.handler_count());

  // When: 成员函数做委托函数
  Example example;
  button.OnClick += std::bind(&Example::Click, example, std::placeholders::_1);

  // Then
  EXPECT_EQ(2, button.OnClick.handler_count());

  // When: 匿名函数做委托函数
  button.OnClick += [](int& result) {
    checker = 300;
    result = checker;
    std::cout << "Lambda Click(result=" << result << ")" << std::endl;
  };

  // Then
  EXPECT_EQ(3, button.OnClick.handler_count());
}

TEST_F(Event_Test_Fixture, clear_handler) {
  // Given

  // 静态函数做委托函数
  button.OnClick += Click;

  // 成员函数做委托函数
  Example example;
  button.OnClick += std::bind(&Example::Click, example, std::placeholders::_1);

  // 匿名函数做委托函数
  button.OnClick += [](int& result) {
    checker = 300;
    result = checker;
    std::cout << "Lambda Click(result=" << result << ")" << std::endl;
  };
  EXPECT_EQ(3, button.OnClick.handler_count());
  
  // When
  button.OnClick.clear_handler();
  // Then
  EXPECT_EQ(0, button.OnClick.handler_count());
}

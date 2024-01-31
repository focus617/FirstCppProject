#include "event/event_dispatcher.hpp"

#include <gtest/gtest.h>

#include "event/key_event.hpp"

static bool checker = false;

using namespace xuzy;

// 一个能够生产事件的类
class Button {
 public:
  Button() {}
  virtual ~Button() {}

 public:
  // EventHandler:  void(*func)(int& result)
  xuzy::EventDispatcher<void()> eventDispatcher;
};

// 一个能够处理事件的静态函数
void EventHandler(Ref<IEvent> evt, bool& handled) {
  checker = true;  // modify global static variable

  handled = false;
  if (evt->IsInCategory(EventCategoryKeyboard)) {
    switch (evt->GetEventId()) {
      case EventId::KeyPressed:
        // Ref<KeyPressedEvent> event = dynamic_cast<Ref<KeyPressedEvent>>(evt);
        std::cout << "Button Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  } else {
    std::cout << "Event other than Keyboard : " << *evt << std::endl;
  }
}

// 一个接收事件的类，并且它具有能够处理事件的方法
class Example {
 public:
  void EventHandler(Ref<IEvent> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->GetEventId()) {
      case EventId::KeyPressed:
        std::cout << "Example Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  }
};

////////////////////////////////////////////////////////

class EventDispatcher_Test_Fixture : public testing::Test {
 public:
  Button button;
  Ref<IEvent> event;

  EventDispatcher_Test_Fixture() {}
  ~EventDispatcher_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

// 生产KeyPressed事件
void EventDispatcher_Test_Fixture::SetUp() {
  event = CreateRef<KeyPressedEvent>(KeyPressedEvent(Key::Space, 1));
  button.eventDispatcher.set_event(event);
}

void EventDispatcher_Test_Fixture::TearDown() {}

TEST_F(EventDispatcher_Test_Fixture, event_on_static_func) {
  // Given
  checker = false;

  // When: 静态函数做委托函数
  button.eventDispatcher += EventHandler;
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->Handled);

  // Then
  button.eventDispatcher();
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->Handled);
}

TEST_F(EventDispatcher_Test_Fixture, event_on_class_member_func) {
  // Given
  checker = false;

  // When: 成员函数做委托函数
  Example example;
  button.eventDispatcher +=
      std::bind(&Example::EventHandler, example, std::placeholders::_1,
                std::placeholders::_2);
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->Handled);

  // Then
  button.eventDispatcher();
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->Handled);
}

TEST_F(EventDispatcher_Test_Fixture, event_on_lambda_func) {
  // Given
  checker = false;

  // When: 匿名函数做委托函数
  button.eventDispatcher += [](Ref<IEvent> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->GetEventId()) {
      case EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->Handled);

  // Then
  button.eventDispatcher();
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->Handled);
}

TEST_F(EventDispatcher_Test_Fixture, handler_count) {
  // Given

  // When: 静态函数做委托函数
  button.eventDispatcher += EventHandler;
  // Then
  EXPECT_EQ(1, button.eventDispatcher.handler_count());

  // When: 成员函数做委托函数
  Example example;
  button.eventDispatcher +=
      std::bind(&Example::EventHandler, example, std::placeholders::_1,
                std::placeholders::_2);

  // Then
  EXPECT_EQ(2, button.eventDispatcher.handler_count());

  // When: 匿名函数做委托函数
  button.eventDispatcher += [](Ref<IEvent> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->GetEventId()) {
      case EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };

  // Then
  EXPECT_EQ(3, button.eventDispatcher.handler_count());
}

TEST_F(EventDispatcher_Test_Fixture, clear_handler) {
  // Given

  // 静态函数做委托函数
  button.eventDispatcher += EventHandler;

  // 成员函数做委托函数
  Example example;
  button.eventDispatcher +=
      std::bind(&Example::EventHandler, example, std::placeholders::_1,
                std::placeholders::_2);

  // 匿名函数做委托函数
  button.eventDispatcher += [](Ref<IEvent> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->GetEventId()) {
      case EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };
  EXPECT_EQ(3, button.eventDispatcher.handler_count());

  // When
  button.eventDispatcher.clear_handler();
  // Then
  EXPECT_EQ(0, button.eventDispatcher.handler_count());
}

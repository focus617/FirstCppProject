#include "tools/event/event_dispatcher.hpp"

#include <gtest/gtest.h>

#include "tools/event/key_event.hpp"

static bool checker = false;

using namespace xuzy;

// 一个能够生产事件的类
class Button {
 public:
  Button() {}
  virtual ~Button() {}

 public:
  // EventHandler:  void(*func)(oid(Ref<Event>, bool&)
  Events::EventDispatcher<void()> event_dispatcher;
};

// 一个能够处理事件的静态函数
void EventHandler(Ref<Events::Event> evt, bool& handled) {
  checker = true;  // modify global static variable

  handled = false;
  if (evt->is_in_category(Events::EventCategoryKeyboard)) {
    switch (evt->get_event_id()) {
      case Events::EventId::KeyPressed:
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
  void EventHandler(Ref<Events::Event> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->get_event_id()) {
      case Events::EventId::KeyPressed:
        std::cout << "Example Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  }
};

#define EXAMPLE_BIND_EVENT_FN(entity)                              \
  std::bind(&Example::EventHandler, entity, std::placeholders::_1, \
            std::placeholders::_2)

////////////////////////////////////////////////////////

class EventDispatcher_Test_Fixture : public testing::Test {
 public:
  Button button;
  Ref<Events::Event> event;

  EventDispatcher_Test_Fixture() {}
  ~EventDispatcher_Test_Fixture() {}

  void SetUp();
  void TearDown();
};

// 生产KeyPressed事件
void EventDispatcher_Test_Fixture::SetUp() {
  event = CreateRef<Events::KeyPressedEvent>(
      Events::KeyPressedEvent(Window::Inputs::Key::Space, 1));
}

void EventDispatcher_Test_Fixture::TearDown() {}

TEST_F(EventDispatcher_Test_Fixture, event_on_static_func) {
  // Given
  checker = false;

  // When: 静态函数做委托函数
  button.event_dispatcher += EventHandler;
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->m_handled);

  // Then
  button.event_dispatcher.dispatch(event);
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->m_handled);
}

TEST_F(EventDispatcher_Test_Fixture, event_on_class_member_func) {
  // Given
  checker = false;

  // When: 成员函数做委托函数
  Example example;
  button.event_dispatcher += EXAMPLE_BIND_EVENT_FN(example);
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->m_handled);

  // Then
  button.event_dispatcher.dispatch(event);
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->m_handled);
}

TEST_F(EventDispatcher_Test_Fixture, event_on_lambda_func) {
  // Given
  checker = false;

  // When: 匿名函数做委托函数
  button.event_dispatcher += [](Ref<Events::Event> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->get_event_id()) {
      case Events::EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };
  EXPECT_EQ(false, checker);
  EXPECT_EQ(false, event->m_handled);

  // Then
  button.event_dispatcher.dispatch(event);
  EXPECT_EQ(true, checker);
  EXPECT_EQ(true, event->m_handled);
}

TEST_F(EventDispatcher_Test_Fixture, handler_count) {
  // Given

  // When: 静态函数做委托函数
  button.event_dispatcher += EventHandler;
  // Then
  EXPECT_EQ(1, button.event_dispatcher.handler_count());

  // When: 成员函数做委托函数
  Example example;
  button.event_dispatcher += EXAMPLE_BIND_EVENT_FN(example);

  // Then
  EXPECT_EQ(2, button.event_dispatcher.handler_count());

  // When: 匿名函数做委托函数
  button.event_dispatcher += [](Ref<Events::Event> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->get_event_id()) {
      case Events::EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };

  // Then
  EXPECT_EQ(3, button.event_dispatcher.handler_count());
}

TEST_F(EventDispatcher_Test_Fixture, clear_handler) {
  // Given

  // 静态函数做委托函数
  button.event_dispatcher += EventHandler;

  // 成员函数做委托函数
  Example example;
  button.event_dispatcher += EXAMPLE_BIND_EVENT_FN(example);

  // 匿名函数做委托函数
  button.event_dispatcher += [](Ref<Events::Event> evt, bool& handled) {
    checker = true;  // modify global static variable

    handled = false;
    switch (evt->get_event_id()) {
      case Events::EventId::KeyPressed:
        std::cout << "Lambda Click(Event: " << *evt << ")" << std::endl;
        handled = true;
        break;
      default:
        std::cout << "Other Event: " << *evt << std::endl;
        break;
    }
  };
  EXPECT_EQ(3, button.event_dispatcher.handler_count());

  // When
  button.event_dispatcher.clear_handlers();
  // Then
  EXPECT_EQ(0, button.event_dispatcher.handler_count());
}

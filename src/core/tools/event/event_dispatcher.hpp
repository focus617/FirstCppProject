#pragma once

#include "pch.h"

#include <assert.h>

#include "event.hpp"

namespace xuzy {

/**
 * The ID of a listener/delegater (Registered handler/callback).
 * This value is needed to remove a listener/delegater from an event
 */
using HandlerID = uint64_t;

static HandlerID DELEGATE_ID = 1;

/**
 * @brief prototype
 */
template <typename Prototype>
class EventDispatcher;

/**
 * @brief 特例: 对事件类Event进行了模板化
 * 
 * 
 * 使用变参模板自定义事件绑定的委托函数参数列表，可以接受多个不同类型的参数。
 */
template <typename ReturnType, typename... Args>
class EventDispatcher<ReturnType(Args...)> {
 private:
  using return_type = ReturnType;
  using function_type = ReturnType(Ref<Event>, bool&, Args...);
  using std_function_type = std::function<function_type>;

  /**
   * @brief Delegater for event handler
   */
  class EventHandler {
   public:
    EventHandler() = delete;

    EventHandler(std_function_type handler) {
      assert(handler != nullptr);
      m_Handler_ = handler;
    }

    void invoke(Ref<Event> event, bool& handled, Args... args) {
      if (m_Handler_ != nullptr) {
        m_Handler_(event, handled, args...);
      }
    }

   private:
    std_function_type m_Handler_;
  };

 public:
  EventDispatcher() {}
  virtual ~EventDispatcher() = default;

  /**
   * @brief Dispatch event to all registered event handler
   */
  void dispatch(Ref<Event> event, Args... args);

  /**
   * @brief Dispatch event to specific registered event handler
   */
  bool dispatch(HandlerID handler_id, Ref<Event> event, Args... args);

  /**
   * @brief Dispatch event to specific registered event handler
   */
  int handler_count();

  /**
   * @brief Dispatch event to specific registered event handler
   */
  void clear_handlers();

  /**
   * Add a function handler/callback to this event
   * Also return the ID of the new listener (You should store the returned ID if
   * you want to remove the listener later)
   * @param handler
   */
  HandlerID subscribe(std_function_type handler);

  /**
   * Remove a function callback/handler to this event using a Listener
   * (Created when calling subscribe)
   * @param handler_id
   */
  bool unsubscribe(HandlerID handler_id);

  /**
   * @brief Simple shortcut
   */
  HandlerID operator+=(std_function_type handler);
  bool operator-=(HandlerID handler_id);

 private:
  std::map<HandlerID, Ref<EventHandler>> m_handlers_;
  std::mutex m_mutex_;
};

}  // namespace xuzy

#include "event_dispatcher.inl"

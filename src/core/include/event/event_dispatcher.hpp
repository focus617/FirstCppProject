#pragma once
#include "pch.h"

#include <assert.h>

#include "event/event.hpp"

namespace xuzy {

#define DELEGATE_ID_TYPE std::atomic_uint64_t

static DELEGATE_ID_TYPE DELEGATE_ID = 1;

/**
 * @brief prototype
 */
template <typename Prototype>
class EventDispatcher;

/**
 * @brief 特例: 对事件类Event进行了模板化
 * 使用变参模板自定义事件绑定的委托函数参数列表，可以接受多个不同类型的参数。
 */
template <typename ReturnType, typename... Args>
class EventDispatcher<ReturnType(Args...)> {
 private:
  using return_type = ReturnType;
  using function_type = ReturnType(Ref<Event>, bool&, Args...);
  using std_function_type = std::function<function_type>;
  using pointer = ReturnType (*)(Args...);

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
   * @brief cache a event to dispatcher
   */
  void publish_event(Ref<Event> event) { m_event_ = event; }

  /**
   * @brief dispatch event to all registered event handler
   */
  void dispatch(Args... args) {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    for (const auto& key : m_Handlers_) {
      bool handled = false;
      key.second->invoke(m_event_, handled, args...);
      m_event_->m_handled |= handled;
    }
  }

  bool dispatch(int handler_id, Args... args) {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    if (m_Handlers_.count(handler_id) == 0) return false;

    bool handled = false;
    m_Handlers_[handler_id]->invoke(m_event_, handled, args...);
    m_event_->m_handled |= handled;

    return true;
  }

  // 这里 std_function_type 决定了所有Handler的返回值和参数都必须是一致的
  int operator+=(std_function_type handler) { return subscribe(handler); }

  // TODO: 使用ID不方便，能否找到比较简单清晰的注册函数的方法？
  bool operator-=(int handler_id) { return unsubscribe(handler_id); }

  int handler_count() {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);
    return m_Handlers_.size();
  }

  void clear_handler() {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);
    m_Handlers_.clear();
  }

  int subscribe(std_function_type handler) {
    if (nullptr == handler) return -1;

    Ref<EventHandler> pEventHandler = CreateRef<EventHandler>(handler);
    if (nullptr == pEventHandler) return -1;

    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    m_Handlers_.insert(std::pair<int, std::shared_ptr<EventHandler>>(
        DELEGATE_ID, pEventHandler));
    return DELEGATE_ID++;
  }

  bool unsubscribe(int handler_id) {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    if (m_Handlers_.count(handler_id) == 0) return false;

    m_Handlers_.erase(handler_id);

    return true;
  }
  
 private:
  // 待处理的事件，需要外部构造后传入
  Ref<Event> m_event_;
  std::map<int, Ref<EventHandler>> m_Handlers_;
  std::mutex m_mutex_;

};

}  // namespace xuzy

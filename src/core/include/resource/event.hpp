#pragma once

#include <assert.h>

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <type_traits>

namespace xuzy {

#define DELEGATE_ID_TYPE std::atomic_uint64_t

static DELEGATE_ID_TYPE DELEGATE_ID = 1;

/**
 * @brief prototype
 */
template <typename Prototype>
class Event;

/**
 * @brief 特例: 对事件类Event进行了模板化
 * 使用变参模板自定义事件绑定的委托函数参数列表，可以接受多个不同类型的参数。
 */
template <typename ReturnType, typename... Args>
class Event<ReturnType(Args...)> {
 private:
  using return_type = ReturnType;
  using function_type = ReturnType(Args...);
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

    void invoke(Args... args) {
      if (m_Handler_ != nullptr) {
        m_Handler_(args...);
      }
    }

   private:
    std_function_type m_Handler_;
  };

 public:
  // 这里 std_function_type 决定了所有Handler的返回值和参数都必须是一致的
  int operator+=(std_function_type handler) { return subscribe(handler); }

  // TODO: 使用ID不方便，能否找到比较简单清晰的注册函数的方法？
  bool operator-=(int handler_id) { return unsubscribe(handler_id); }

  void operator()(Args... args) { invoke(args...); }

  int handler_count() {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);
    return m_Handlers_.size();
  }

  void clear_handler() {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);
    m_Handlers_.clear();
  }

 private:
  std::map<int, std::shared_ptr<EventHandler>> m_Handlers_;
  std::mutex m_mutex_;

  int subscribe(std_function_type handler) {
    if (nullptr == handler) return -1;

    std::shared_ptr<EventHandler> pEventHandler =
        std::make_shared<EventHandler>(handler);
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

  void invoke(Args... args) {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    for (const auto& key : m_Handlers_) {
      key.second->invoke(args...);
    }
  }

  bool invoke(int handler_id, Args... args) {
    std::lock_guard<std::mutex> guard_mutex(m_mutex_);

    if (m_Handlers_.count(handler_id) == 0) return false;

    m_Handlers_[handler_id]->invoke(args...);
    return true;
  }
};

}  // namespace xuzy

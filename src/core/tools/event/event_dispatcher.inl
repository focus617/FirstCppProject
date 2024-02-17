#pragma once

#include "event.hpp"

namespace xuzy::Events {

template <typename ReturnType, typename... Args>
void EventDispatcher<ReturnType(Args...)>::dispatch(Ref<Event> event,
                                                    Args... args) {
  std::lock_guard<std::mutex> guard_mutex(m_mutex_);

  if (m_handlers_.empty()) return;

  for (const auto& [key, handler] : m_handlers_) {
    bool handled = false;
    handler->invoke(event, handled, args...);
    event->m_handled |= handled;
  }
}

template <typename ReturnType, typename... Args>
bool EventDispatcher<ReturnType(Args...)>::dispatch(HandlerID handler_id,
                                                    Ref<Event> event,
                                                    Args... args) {
  std::lock_guard<std::mutex> guard_mutex(m_mutex_);

  if (m_handlers_.count(handler_id) == 0) return false;

  bool handled = false;
  m_handlers_[handler_id]->invoke(event, handled, args...);
  event->m_handled |= handled;

  return true;
}

template <typename ReturnType, typename... Args>
int EventDispatcher<ReturnType(Args...)>::handler_count() {
  std::lock_guard<std::mutex> guard_mutex(m_mutex_);
  return m_handlers_.size();
}

template <typename ReturnType, typename... Args>
void EventDispatcher<ReturnType(Args...)>::clear_handlers() {
  std::lock_guard<std::mutex> guard_mutex(m_mutex_);
  m_handlers_.clear();
}

template <typename ReturnType, typename... Args>
HandlerID EventDispatcher<ReturnType(Args...)>::subscribe(
    std_function_type handler) {
  // 这里 std_function_type 决定了所有Handler的返回值和参数都必须是一致的
  if (nullptr == handler) return -1;

  Ref<EventHandler> pEventHandler = CreateRef<EventHandler>(handler);
  if (nullptr == pEventHandler) return -1;

  std::lock_guard<std::mutex> guard_mutex(m_mutex_);
  HandlerID id = DELEGATE_ID++;
  m_handlers_.insert(
      std::pair<HandlerID, std::shared_ptr<EventHandler>>(id, pEventHandler));
  return id;
}

template <typename ReturnType, typename... Args>
bool EventDispatcher<ReturnType(Args...)>::unsubscribe(HandlerID handler_id) {
  std::lock_guard<std::mutex> guard_mutex(m_mutex_);
  return m_handlers_.erase(handler_id) != 0;
}

template <typename ReturnType, typename... Args>
HandlerID EventDispatcher<ReturnType(Args...)>::operator+=(
    std_function_type handler) {
  return subscribe(handler);
}

template <typename ReturnType, typename... Args>
bool EventDispatcher<ReturnType(Args...)>::operator-=(HandlerID handler_id) {
  return unsubscribe(handler_id);
}

}  // namespace xuzy::Events

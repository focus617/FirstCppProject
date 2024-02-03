#pragma once

#include <sstream>

#include "core/key_code.hpp"
#include "event/event.hpp"

namespace xuzy {

class KeyEvent : public Event {
 public:
  KeyCode GetKeyCode() const { return m_KeyCode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

 protected:
  KeyEvent(const KeyCode keycode) : m_KeyCode(keycode) {}

  KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
      : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

  uint16_t GetRepeatCount() const { return m_RepeatCount; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent:" << m_KeyCode << " (" << m_RepeatCount
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_ID(KeyPressed)
  
 private:
  uint16_t m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent:" << m_KeyCode;
    return ss.str();
  }

  EVENT_CLASS_ID(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
 public:
  KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent:" << m_KeyCode;
    return ss.str();
  }

  EVENT_CLASS_ID(KeyTyped)
};

}  // namespace xuzy
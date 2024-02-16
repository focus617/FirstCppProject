#pragma once

#include <sstream>

#include "event.hpp"
#include "window/inputs/key_codes.h"

namespace xuzy {

class KeyEvent : public Event {
 public:
  xuzy::Window::Inputs::KeyCode get_key_code() const { return m_key_code; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

 protected:
  KeyEvent(const xuzy::Window::Inputs::KeyCode keycode) : m_key_code(keycode) {}

  xuzy::Window::Inputs::KeyCode m_key_code;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(const xuzy::Window::Inputs::KeyCode keycode,
                  const uint16_t repeatCount)
      : KeyEvent(keycode), m_repeat_count(repeatCount) {}

  uint16_t get_repeat_count() const { return m_repeat_count; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent:" << m_key_code << " (" << m_repeat_count
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_ID(KeyPressed)

 private:
  uint16_t m_repeat_count;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(const xuzy::Window::Inputs::KeyCode keycode)
      : KeyEvent(keycode) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent:" << m_key_code;
    return ss.str();
  }

  EVENT_CLASS_ID(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
 public:
  KeyTypedEvent(const xuzy::Window::Inputs::KeyCode keycode)
      : KeyEvent(keycode) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent:" << m_key_code;
    return ss.str();
  }

  EVENT_CLASS_ID(KeyTyped)
};

}  // namespace xuzy
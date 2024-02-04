#pragma once

#include <sstream>

#include "core/mouse_codes.hpp"
#include "event/event.hpp"

namespace xuzy {

class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(const float x, const float y) : m_mouse_x(x), m_mouse_y(y) {}

  float get_x() const { return m_mouse_x; }
  float get_y() const { return m_mouse_y; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
    return ss.str();
  }

  EVENT_CLASS_ID(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

 private:
  float m_mouse_x, m_mouse_y;
};

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(const float xOffset, const float yOffset)
      : m_x_offset(xOffset), m_y_offset(yOffset) {}

  float get_x_offset() const { return m_x_offset; }
  float get_y_offset() const { return m_y_offset; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << get_x_offset() << ", " << get_y_offset();
    return ss.str();
  }

  EVENT_CLASS_ID(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

 private:
  float m_x_offset, m_y_offset;
};

class MouseButtonEvent : public Event {
 public:
  MouseCode get_mouse_button() const { return m_button; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput |
                       EventCategoryMouseButton)

 protected:
  MouseButtonEvent(const MouseCode button) : m_button(button) {}

  MouseCode m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << m_button;
    return ss.str();
  }

  EVENT_CLASS_ID(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << m_button;
    return ss.str();
  }

  EVENT_CLASS_ID(MouseButtonReleased)
};

}  // namespace xuzy

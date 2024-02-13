#pragma once

#include <sstream>

#include "core/base.hpp"
#include "event.hpp"
#include "window/API/IDrawable.h"

namespace xuzy {

class DrawableEvent : public Event {
 public:
  EVENT_CLASS_CATEGORY(EventCategoryDrawable | EventCategoryInput)

 protected:
  DrawableEvent(Ref<Window::API::IDrawable> publisher)
      : m_publisher(publisher) {}

  Ref<Window::API::IDrawable> m_publisher;
};

class DrawableClickedEvent : public DrawableEvent {
 public:
  DrawableClickedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableClickedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableClicked)
};

class DrawableOpenedEvent : public DrawableEvent {
 public:
  DrawableOpenedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableOpenedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableOpened)
};

class DrawableClosedEvent : public DrawableEvent {
 public:
  DrawableClosedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableClosedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableClosed)
};

class MenuValueChangedEvent : public DrawableEvent {
 public:
  MenuValueChangedEvent(Ref<Window::API::IDrawable> publisher, bool p_checked)
      : DrawableEvent(publisher), m_checked(p_checked) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "MenuValueChangedEvent(checked: " << m_checked << ")";
    return ss.str();
  }

  EVENT_CLASS_ID(MenuValueChanged)

 private:
  bool m_checked;
};

}  // namespace xuzy
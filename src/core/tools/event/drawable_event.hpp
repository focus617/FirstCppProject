#pragma once

#include <sstream>

#include "core/base.hpp"
#include "event.hpp"
#include "window/API/IDrawable.h"

namespace xuzy::Events {

class DrawableEvent : public Event {
 public:
  EVENT_CLASS_CATEGORY(EventCategoryDrawable | EventCategoryInput)

 protected:
  DrawableEvent(Ref<Window::API::IDrawable> publisher)
      : m_publisher(publisher) {}

 public:
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

class DrawableDoubleClickedEvent : public DrawableEvent {
 public:
  DrawableDoubleClickedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableDoubleClickedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableDoubleClicked)
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

class DrawableSelectedEvent : public DrawableEvent {
 public:
  DrawableSelectedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableSelectedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableSelected)
};

class DrawableUnselectedEvent : public DrawableEvent {
 public:
  DrawableUnselectedEvent(Ref<Window::API::IDrawable> publisher)
      : DrawableEvent(publisher) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableUnselectedEvent";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableUnselected)
};

template <typename T>
class DrawableValueChangedEvent : public DrawableEvent {
 public:
  DrawableValueChangedEvent(Ref<Window::API::IDrawable> publisher,
                            T p_value)
      : DrawableEvent(publisher), m_value(p_value) {}

  std::string to_string() const override {
    std::stringstream ss;
    ss << "DrawableValueChangedEvent(value: " << m_value << ")";
    return ss.str();
  }

  EVENT_CLASS_ID(DrawableValueChanged)

 private:
  T m_value;
};

}  // namespace xuzy::Events
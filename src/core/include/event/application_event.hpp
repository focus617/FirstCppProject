#pragma once

#include "event/event.hpp"

namespace xuzy {

class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : m_Width(width), m_Height(height) {}

  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  EVENT_CLASS_ID(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
 private:
  unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event {
 public:
  WindowCloseEvent() = default;

  EVENT_CLASS_ID(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppLaunchEvent : public Event {
 public:
  AppLaunchEvent() = default;

  EVENT_CLASS_ID(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppEndEvent : public Event {
 public:
  AppEndEvent() = default;

  EVENT_CLASS_ID(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event {
 public:
  AppTickEvent() = default;

  EVENT_CLASS_ID(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event {
 public:
  AppUpdateEvent() = default;

  EVENT_CLASS_ID(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event {
 public:
  AppRenderEvent() = default;

  EVENT_CLASS_ID(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

}  // namespace xuzy
#pragma once

#include "event/event.hpp"

namespace xuzy {

class WindowMovedEvent : public Event {
 public:
  WindowMovedEvent(int xpos, int ypos)
      : m_xpos(xpos), m_ypos(ypos) {}

  unsigned int get_xpos() const { return m_xpos; }
  unsigned int get_ypos() const { return m_ypos; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "WindowMovedEvent: " << m_xpos << ", " << m_ypos;
    return ss.str();
  }

  EVENT_CLASS_ID(WindowMoved)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
 private:
  int m_xpos, m_ypos;
};

class WindowResizeEvent : public Event {
 public:
  WindowResizeEvent(unsigned int width, unsigned int height)
      : m_width(width), m_height(height) {}

  unsigned int get_width() const { return m_width; }
  unsigned int get_height() const { return m_height; }

  std::string to_string() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_width << ", " << m_height;
    return ss.str();
  }

  EVENT_CLASS_ID(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
 private:
  unsigned int m_width, m_height;
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
#pragma once
#include "core/base.hpp"

namespace xuzy::Events {

// Events are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

/**
 * @brief  Event id
 */
enum class EventId {
  None = 0,
  AppLaunch = 1,
  AppEnd,
  AppTick,
  AppUpdate,
  AppRender,
  WindowClose = 20,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  KeyPressed = 40,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed = 50,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled,
  DrawableClicked = 60,
  DrawableDoubleClicked,
  DrawableOpened,
  DrawableClosed,
  DrawableSelected,
  DrawableUnselected,
  DrawableValueChanged,
  EventIdCount
};

/**
 * @brief  Event category
 * An event id may belong to one or more categories
 */
enum EventCategory {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4),
  EventCategoryDrawable = BIT(5)
};

// get_event_id() is used for dispatching
// get_name() used for debugging purpose
#define EVENT_CLASS_ID(id)                                              \
  static EventId get_static_id() { return EventId::id; }                  \
  virtual EventId get_event_id() const override { return get_static_id(); } \
  virtual const char* get_name() const override { return #id; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int get_category_flags() const override { return category; }

class Event {
 public:
  virtual ~Event() = default;

  bool m_handled = false;

  virtual EventId get_event_id() const = 0;
  virtual const char* get_name() const = 0;
  virtual int get_category_flags() const = 0;
  virtual std::string to_string() const { return get_name(); }

  bool is_in_category(EventCategory category) {
    return get_category_flags() & category;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.to_string();
}

}  // namespace xuzy::Events
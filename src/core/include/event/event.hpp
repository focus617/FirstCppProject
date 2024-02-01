#pragma once
#include "core/base.hpp"

namespace xuzy {

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
  EventCategoryMouseButton = BIT(4)
};

// GetEventId() is used for dispatching
// GetName() used for debugging purpose
#define EVENT_CLASS_ID(id)                                              \
  static EventId GetStaticId() { return EventId::id; }                  \
  virtual EventId GetEventId() const override { return GetStaticId(); } \
  virtual const char* GetName() const override { return #id; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class Event {
 public:
  virtual ~Event() = default;

  bool Handled = false;

  virtual EventId GetEventId() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace xuzy
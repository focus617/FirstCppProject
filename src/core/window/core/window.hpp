/**
 * @brief Platform Independent Window Interface
 */
#pragma once

#include "pch.h"

#include "event/event_dispatcher.hpp"
#include "window/core/window_props.hpp"

namespace xuzy::Window {

/**
 * @brief Platform independent interface representing a desktop system based
 * Window
 */
class XUZY_API Window {
 public:
  // EventHandler Signature:  void(*func)(Ref<Event>, bool&)
  using EventCallbackFn = std::function<void(Ref<Event>, bool&)>;

  /**
   * @brief Destructor of the window
   */
  virtual ~Window() {}

  /**
   * @brief Return the current size of the window
   */
  virtual std::pair<uint16_t, uint16_t> get_size() const = 0;

  // Window attributes
  virtual void set_vsync(bool enabled) = 0;
  virtual bool is_vsync() const = 0;

  virtual void on_update() = 0;

  virtual void set_event_callback(const EventCallbackFn& callback) = 0;

  // EventDispatcher for Window
  xuzy::EventDispatcher<void()> eventDispatcher;

  static Window* Create(const WindowProps& props = WindowProps());
};

// To be defined in concrete window class

}  // namespace xuzy::Window
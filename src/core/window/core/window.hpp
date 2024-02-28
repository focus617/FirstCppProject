/**
 * @brief Platform Independent Window Interface
 */
#pragma once

#include "pch.h"

#include "tools/event/event_dispatcher.hpp"
#include "window/core/window_props.hpp"

namespace xuzy::Window {

/**
 * @brief Platform independent interface representing a desktop system based
 * Window
 */
class XUZY_API AWindow {
 public:
  // EventHandler Signature:  void(*func)(Ref<Events::Event>, bool&)
  using EventCallbackFn = std::function<void(Ref<Events::Event>, bool&)>;

  /**
   * @brief Concrete window creater
   * @param p_props Window Settings
   */
  static Ref<AWindow> Create(const WindowProps& p_props);

  /**
   * @brief Destructor of the window
   */
  virtual ~AWindow() {}

  /**
   * @brief Return the current size of the window
   */
  virtual std::pair<uint16_t, uint16_t> get_size() const = 0;

  virtual void on_update() = 0;

  virtual void set_event_callback(const EventCallbackFn& callback) = 0;

  // EventDispatcher for Window
  Events::EventDispatcher<void()> event_dispatcher;
};

}  // namespace xuzy::Window
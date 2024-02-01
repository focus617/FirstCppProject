/**
 * @brief Platform Independent Window Interface
 */
#pragma once

#include "pch.h"

#include "event/event_dispatcher.hpp"

namespace xuzy {

/**
 * @brief Properties of Window
 */
struct WindowProps {
  std::string Title;
  unsigned int Width;
  unsigned int Height;

  WindowProps(const std::string& title = "Xuzy's App",
              unsigned int width = 1280, unsigned int height = 720)
      : Title(title), Width(width), Height(height) {}
};

/**
 * @brief Platform independent interface representing a desktop system based
 * Window
 */
class XUZY_API Window {
 public:
  // EventHandler Signature:  void(*func)(Ref<Event>, bool&)
  using EventCallbackFn = std::function<void(Ref<Event>, bool&)>;

  virtual ~Window() {}

  virtual void OnUpdate() = 0;

  virtual unsigned int GetWidth() const = 0;
  virtual unsigned int GetHeight() const = 0;

  // Window attributes
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void set_event_callback(const EventCallbackFn& callback) = 0;

  // EventDispatcher for Window
  xuzy::EventDispatcher<void()> eventDispatcher;

  static Window* Create(const WindowProps& props = WindowProps());
};

// To be defined in concrete window class

}  // namespace xuzy
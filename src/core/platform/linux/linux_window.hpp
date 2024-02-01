#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window/window.hpp"

namespace xuzy {

class LinuxWindow : public Window {
 public:
  LinuxWindow(const WindowProps& props);
  virtual ~LinuxWindow();

  void OnUpdate() override;

  inline unsigned int GetWidth() const override { return m_Data.Width; }
  inline unsigned int GetHeight() const override { return m_Data.Height; }

  // Window attributes
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  inline void set_event_callback(const EventCallbackFn& callback) override {
    m_Data.eventDispatcher += callback;
  }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* m_Window;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    xuzy::EventDispatcher<void()> eventDispatcher;
  };

  WindowData m_Data;
};

}  // namespace xuzy
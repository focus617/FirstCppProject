#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window/window.hpp"

namespace xuzy {

class XUZY_API LinuxWindow : public Window {
 public:
  LinuxWindow(const WindowProps& props);
  virtual ~LinuxWindow();

  void on_update() override;

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

  void imgui_setup(const char* glsl_version);
  void imgui_shutdown();
  void imgui_load_fonts();
  void imgui_rendering();

  void draw();

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
#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "window/window.hpp"

namespace xuzy {

class XUZY_API WindowImpl : public Window {
 public:
  WindowImpl(const WindowProps& props);
  virtual ~WindowImpl();

  void on_update() override;

  inline unsigned int get_width() const override { return m_data_.Width; }
  inline unsigned int get_height() const override { return m_data_.Height; }

  // Window attributes
  void set_vsync(bool enabled) override;
  bool is_vsync() const override;

  inline void set_event_callback(const EventCallbackFn& callback) override {
    m_data_.eventDispatcher += callback;
  }

  inline virtual void* get_native_window() const { return p_glfw_window_; }

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  GLFWwindow* p_glfw_window_;

  struct WindowData {
    std::string Title;
    unsigned int Width, Height;
    bool VSync;

    xuzy::EventDispatcher<void()> eventDispatcher;
  };

  WindowData m_data_;
};

}  // namespace xuzy
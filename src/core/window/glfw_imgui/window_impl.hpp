#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "imgui.h"
#include "window/glfw_imgui/imgui_impl_glfw.h"
#include "window/backend/opengl/imgui_impl_opengl3.h"
#include "window/core/window.hpp"

namespace xuzy {

class XUZY_API WindowImpl : public Window {
  struct WindowData;

 public:
  WindowImpl(const WindowProps& props);
  virtual ~WindowImpl();

  virtual void on_update() override;

  inline virtual unsigned int get_width() const override {
    return m_data_.Width;
  }
  inline virtual unsigned int get_height() const override {
    return m_data_.Height;
  }

  // Window attributes
  virtual void set_vsync(bool enabled) override;
  virtual bool is_vsync() const override;

  inline virtual void set_event_callback(
      const EventCallbackFn& callback) override {
    m_data_.eventDispatcher += callback;
  }

  inline const WindowData& get_window_data() const { return m_data_; }
  inline void* get_native_window() const { return p_glfw_window_handle_; }

 private:
  void glfw_window_init();
  void glfw_window_shutdown();

 private:
  GLFWwindow* p_glfw_window_handle_ = nullptr;

  struct WindowData {
    std::string Title;
    int xPos, yPos;
    unsigned int Width, Height;
    bool VSync;

    xuzy::EventDispatcher<void()> eventDispatcher;
  };

  WindowData m_data_;
};

}  // namespace xuzy
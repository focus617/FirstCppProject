#pragma once

#include "pch.h"

#include "app/app.hpp"
#include "tools/event/application_event.hpp"
#include "window/core/layerstack.hpp"
#include "window/core/window.hpp"
#include "window/glfw_imgui/imgui_layer.hpp"

namespace xuzy {

/**
 *  @brief Window Application Scaffold
 */
class XUZY_API WindowApp : public App {
 public:
  XUZY_API explicit WindowApp(const std::string& t_app_name,
                              const std::string& t_version);
  XUZY_API ~WindowApp();

  inline XUZY_API Window::Window& get_window() { return *p_window_; }
  // inline XUZY_API Ref<ImGuiLayer> get_imgui() { return p_imgui_layer; }

  template <typename T>
  void push_layer() {
    static_assert(std::is_base_of<Window::Layer, T>::value,
                  "Pushed type is not subclass of Layer!");
    m_layerstack_.push_layer(CreateRef<T>());
  }

  XUZY_API void push_layer(const Ref<Window::Layer>& layer) {
    m_layerstack_.push_layer(layer);
  }

  XUZY_API void close() { m_running_ = false; }

 protected:
  XUZY_API void launch_tasks() override;
  XUZY_API void main_loop() override;

  XUZY_API virtual void on_event(Ref<Event> evt, bool& handled);
  XUZY_API virtual bool OnWindowClose(Ref<WindowCloseEvent> e);
  XUZY_API virtual bool OnWindowResize(Ref<WindowResizeEvent> e);

 private:
  Scope<Window::Window> p_window_;
  bool m_minimized_ = false;
  bool m_running_ = true;

  Ref<Window::ImGuiLayer> p_imgui_layer;

  Window::LayerStack m_layerstack_;
  std::function<void()> m_menubar_callback_;
};

}  // namespace xuzy
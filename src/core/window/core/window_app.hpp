#pragma once

#include "pch.h"

#include "app/app.hpp"
#include "tools/event/application_event.hpp"
#include "window/core/layerstack.hpp"
#include "window/core/window.hpp"
#include "ui/core/ui_manager.hpp"

namespace xuzy {

/**
 *  @brief Window Application Scaffold
 */
class XUZY_API WindowApp : public App {
 public:
  XUZY_API explicit WindowApp(const std::string& t_app_name,
                              const std::string& t_version);
  XUZY_API ~WindowApp();

  inline XUZY_API Window::AWindow& get_window() { return *m_window_; }
  // inline XUZY_API Ref<UIManager> get_imgui() { return m_ui_manager_; }

  template <typename T>
  void push_layer() {
    static_assert(std::is_base_of<Window::ALayer, T>::value,
                  "Pushed type is not subclass of Layer!");
    m_layerstack_.push_layer(CreateRef<T>());
  }

  XUZY_API void push_layer(const Ref<Window::ALayer>& layer) {
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
  Scope<Window::AWindow> m_window_;
  bool m_minimized_ = false;
  bool m_running_ = true;

  Ref<UI::UIManager> m_ui_manager_;

  Window::LayerStack m_layerstack_;
  std::function<void()> m_menubar_callback_;
};

}  // namespace xuzy
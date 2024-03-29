#pragma once

#include "pch.h"

#include "app/app.hpp"
#include "tools/event/application_event.hpp"
#include "ui/core/ui_manager.hpp"
#include "window/core/app_context.hpp"
#include "window/core/layerstack.hpp"

namespace xuzy {

/**
 *  @brief Window Application Scaffold
 */
class XUZY_API WindowApp : public App {
 public:
  XUZY_API explicit WindowApp(const std::string& p_app_name,
                              const std::string& p_version);
  XUZY_API ~WindowApp();

  XUZY_API Window::AWindow& get_window() {
    return *(m_context_.m_window_);
  }
  // XUZY_API Ref<UIManager> get_imgui() { return m_ui_manager_; }

  template <typename T, typename... Args>
  void push_layer(Args&&... args) {
    static_assert(std::is_base_of<Window::ALayer, T>::value,
                  "Pushed type is not subclass of Layer!");
    m_layerstack_.push_layer(CreateRef<T>(std::forward<Args>(args)...));
  }

  XUZY_API void push_layer(const Ref<Window::ALayer>& layer) {
    m_layerstack_.push_layer(layer);
  }

  XUZY_API void close() { m_running_ = false; }

 protected:
  XUZY_API void launch_tasks() override;
  XUZY_API void main_loop() override;

  XUZY_API virtual void on_event(Ref<Events::Event> evt, bool& handled);
  XUZY_API virtual bool OnWindowClose(Ref<Events::WindowCloseEvent> e);
  XUZY_API virtual bool OnWindowResize(Ref<Events::WindowResizeEvent> e);

 private:
  Window::AppContext m_context_;

  bool m_minimized_ = false;
  bool m_running_ = true;

  Ref<UI::UIManager> m_ui_manager_;

  Window::LayerStack m_layerstack_;
  float m_last_frame_time_ = 0.0f;
};

}  // namespace xuzy
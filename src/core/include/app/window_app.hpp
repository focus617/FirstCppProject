#pragma once

#include "pch.h"

#include "app/app.hpp"
#include "event/application_event.hpp"
#include "window/layerstack.hpp"
#include "window/window.hpp"

namespace xuzy {
/**
 *  @brief Window Application Scaffold
 */
class XUZY_API WindowApp : public App {
 public:
  XUZY_API explicit WindowApp(const std::string& t_app_name,
                              const std::string& t_version);
  XUZY_API ~WindowApp();

  inline XUZY_API Window& get_window() { return *m_window_; }

  XUZY_API void push_layer(Layer* layer);
  XUZY_API void push_overlay(Layer* layer);

  XUZY_API virtual void on_event(Ref<Event> evt, bool& handled);

 protected:
  XUZY_API virtual bool OnWindowClose(Ref<WindowCloseEvent> e);

  XUZY_API void launch_tasks() override;
  XUZY_API void main_loop() override;

 private:
  Scope<Window> m_window_;
  bool m_running_ = true;
  LayerStack m_layerstack_;
};

}  // namespace xuzy
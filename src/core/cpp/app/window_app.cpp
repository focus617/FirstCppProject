#include "app/window_app.hpp"

namespace xuzy {

#define BIND_EVENT_FN(x) \
  std::bind(&WindowApp::x, this, std::placeholders::_1, std::placeholders::_2)

WindowApp::WindowApp(const std::string& t_app_name,
                     const std::string& t_version)
    : App{std::move(t_app_name), std::move(t_version)} {
  p_window_ = std::unique_ptr<Window>(Window::Create());
  p_window_->set_event_callback(BIND_EVENT_FN(on_event));
}

WindowApp::~WindowApp() {}

void WindowApp::push_layer(Layer* layer) {
  m_layerstack_.push_layer(layer);
  layer->on_attach();
}

void WindowApp::push_overlay(Layer* layer) {
  m_layerstack_.push_overlay(layer);
  layer->on_attach();
}

void WindowApp::pop_layer(Layer* layer) {
  m_layerstack_.pop_layer(layer);
  layer->on_detach();
}

void WindowApp::pop_overlay(Layer* layer) {
  m_layerstack_.pop_overlay(layer);
  layer->on_detach();
}

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  while (m_running_) {
    for (Layer* layer : m_layerstack_) layer->on_update();

    p_window_->on_update();
  }
}

void WindowApp::on_event(Ref<Event> event, bool& handled) {
  handled = false;

  // Handle global event, e.g. WindowCloseEvent
  switch (event->get_event_id()) {
    case EventId::WindowClose:
      handled =
          OnWindowClose(std::static_pointer_cast<WindowCloseEvent>(event));
      return;

    default:
      LOG(INFO) << "Other Event: " << *event << std::endl;
      break;
  }

  // 如果不是全局事件，转交LayerStack进行处理
  if (!handled) {
    for (auto it = m_layerstack_.end(); it != m_layerstack_.begin();) {
      (*--it)->on_event(event, handled);
      if (handled) break;
    }
  }
}

bool WindowApp::OnWindowClose(Ref<WindowCloseEvent> e) {
  LOG(INFO) << "Window Close Clicked (Event: " << *e << ")" << std::endl;
  m_running_ = false;
  return true;
}

}  // namespace xuzy
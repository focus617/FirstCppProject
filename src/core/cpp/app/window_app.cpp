#include "app/window_app.hpp"

namespace xuzy {

#define BIND_EVENT_FN(x) \
  std::bind(&WindowApp::x, this, std::placeholders::_1, std::placeholders::_2)

WindowApp::WindowApp(const std::string& t_app_name,
                     const std::string& t_version)
    : App{std::move(t_app_name), std::move(t_version)} {
  m_window_ = std::unique_ptr<Window>(Window::Create());
  m_window_->set_event_callback(BIND_EVENT_FN(on_event));
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

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  while (m_running_) {
    for (Layer* layer : m_layerstack_) layer->on_update();

    m_window_->on_update();
  }
}

void WindowApp::on_event(Ref<Event> event, bool& handled) {
  handled = false;

  // Handle global event, e.g. WindowCloseEvent
  switch (event->get_event_id()) {
    case EventId::WindowClose:
      LOG(INFO) << "Window Close Clicked (Event: " << *event << ")"
                << std::endl;
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
  m_running_ = false;
  return true;
}

}  // namespace xuzy
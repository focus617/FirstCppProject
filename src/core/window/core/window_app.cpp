#include "window_app.hpp"

#include "window/glfw_imgui/example_layer.hpp"

namespace xuzy {

#define BIND_EVENT_FN(x) \
  std::bind(&WindowApp::x, this, std::placeholders::_1, std::placeholders::_2)

WindowApp::WindowApp(const std::string& t_app_name,
                     const std::string& t_version)
    : App{std::move(t_app_name), std::move(t_version)} {
  p_window_ = std::unique_ptr<Window::Window>(
      Window::Window::Create(Window::WindowProps(t_app_name)));
  p_window_->set_event_callback(BIND_EVENT_FN(on_event));

  p_imgui_layer = CreateRef<Window::ImGuiLayer>();
  m_layerstack_.push_overlay(p_imgui_layer);

  push_layer<Window::ExampleLayer>();
}

WindowApp::~WindowApp() { close(); }

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  while (m_running_) {
    if (!m_minimized_) {
      for (Ref<Window::Layer> layer : m_layerstack_) layer->on_update();

      p_imgui_layer->begin_render();
      for (Ref<Window::Layer> layer : m_layerstack_) layer->on_imgui_render();
      p_imgui_layer->end_render();
    }
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

    case EventId::WindowResize:
      handled =
          OnWindowResize(std::static_pointer_cast<WindowResizeEvent>(event));
      break;
      ;

    default:
      LOG(INFO) << "Other Event: " << *event << std::endl;
      break;
  }

  // 如果不是全局事件，转交LayerStack，按从后往前的层顺序处理事件
  if (!handled) {
    for (auto it = m_layerstack_.rbegin(); it != m_layerstack_.rend(); ++it) {
      (*it)->on_event(event, handled);
      if (handled) break;
    }
  }
}

bool WindowApp::OnWindowClose(Ref<WindowCloseEvent> e) {
  LOG(INFO) << "Window Close Clicked (Event: " << *e << ")" << std::endl;
  m_running_ = false;
  return true;
}

bool WindowApp::OnWindowResize(Ref<WindowResizeEvent> e) {
  LOG(INFO) << "Window Resized (Event: " << *e << ")" << std::endl;

  if ((e->get_width() == 0) || (e->get_height() == 0)) {
    m_minimized_ = true;
    return false;
  }

  m_minimized_ = false;

  return false;
}

}  // namespace xuzy
#include "window_app.hpp"

#include "ui/core/canvas.hpp"
#include "ui/panels/panel_menubar.hpp"
#include "window/backend_glfw/window_impl.hpp"

namespace xuzy {

#define BIND_EVENT_FN(x) \
  std::bind(&WindowApp::x, this, std::placeholders::_1, std::placeholders::_2)

WindowApp::WindowApp(const std::string& t_app_name,
                     const std::string& t_version)
    : App{std::move(t_app_name), std::move(t_version)} {
  m_window_ = std::unique_ptr<Window::AWindow>(
      Window::AWindow::Create(Window::WindowProps(t_app_name)));

  m_window_->set_event_callback(BIND_EVENT_FN(on_event));

  Window::WindowImpl& window = (Window::WindowImpl&)get_window();
  GLFWwindow* glfw_window = window.get_native_window();

  // Decide GL+GLSL versions
  std::string glsl_version = "#version 460";  // GL 4.6

  m_ui_manager_ = CreateRef<UI::UIManager>(glfw_window, glsl_version,
                                              UI::Style::IMGUI_DARK_STYLE);
  m_layerstack_.push_overlay(m_ui_manager_);

  Ref<UI::Canvas> canvas = CreateRef<UI::Canvas>();

  m_layerstack_.push_layer(canvas);
}

WindowApp::~WindowApp() { close(); }

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  while (m_running_) {
    if (!m_minimized_) {
      for (Ref<Window::ALayer> layer : m_layerstack_) layer->on_update();

      m_ui_manager_->begin_render();
      for (Ref<Window::ALayer> layer : m_layerstack_) layer->on_draw();
      m_ui_manager_->end_render();
    }
    m_window_->on_update();
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
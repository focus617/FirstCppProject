#include "window_app.hpp"

#include "renderer/renderer/renderer.hpp"
#include "renderer/renderer/renderer2D.hpp"

namespace xuzy {

#define BIND_EVENT_FN(x) \
  std::bind(&WindowApp::x, this, std::placeholders::_1, std::placeholders::_2)

WindowApp::WindowApp(const std::string& p_app_name,
                     const std::string& p_version)
    : App{p_app_name, p_version}, m_context_{p_app_name, p_version} {
  // Note: Window was created inside m_context_
  XUZY_PROFILE_FUNCTION();

  // Setup event callback for Window::AWindow
  m_context_.m_window_->set_event_callback(BIND_EVENT_FN(on_event));

  m_ui_manager_ = CreateRef<UI::UIManager>(m_context_.get_native_window(),
                                           m_context_.glsl_version,
                                           UI::Style::IMGUI_DARK_STYLE);

  m_ui_manager_->set_canvas(CreateRef<UI::Canvas>());

  m_layerstack_.push_layer(m_ui_manager_);

  Renderer::Renderer::init();
  Renderer::Renderer2D::init();
}

WindowApp::~WindowApp() {
  XUZY_PROFILE_FUNCTION();

  Renderer::Renderer2D::shutdown();
  close();
}

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  XUZY_PROFILE_FUNCTION();

  while (m_running_) {
    XUZY_PROFILE_SCOPE("WindowApp::run_loop");

    float time = (float)glfwGetTime();
    Renderer::Times::Timestep timestep = time - m_last_frame_time_;
    m_last_frame_time_ = time;

    if (!m_minimized_) {
      {
        XUZY_PROFILE_SCOPE("LayerStack on_draw");

        m_ui_manager_->begin_render();
        m_layerstack_.on_draw();
        m_ui_manager_->end_render();
      }
      {
        XUZY_PROFILE_SCOPE("LayerStack on_update");
        for (Ref<Window::ALayer> layer : m_layerstack_)
          layer->on_update(timestep);
      }

      m_context_.m_window_->on_update();
    }
  }
}

void WindowApp::on_event(Ref<Events::Event> event, bool& handled) {
  XUZY_PROFILE_FUNCTION();

  handled = false;

  // Handle global event, e.g. WindowCloseEvent
  switch (event->get_event_id()) {
    case Events::EventId::WindowClose:
      handled = OnWindowClose(
          std::static_pointer_cast<Events::WindowCloseEvent>(event));
      return;

    case Events::EventId::WindowResize:
      handled = OnWindowResize(
          std::static_pointer_cast<Events::WindowResizeEvent>(event));
      break;

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

bool WindowApp::OnWindowClose(Ref<Events::WindowCloseEvent> e) {
  XUZY_PROFILE_FUNCTION();

  LOG(INFO) << "Window Close Clicked Event: " << *e << std::endl;
  m_running_ = false;
  return true;
}

bool WindowApp::OnWindowResize(Ref<Events::WindowResizeEvent> e) {
  XUZY_PROFILE_FUNCTION();

  LOG(INFO) << "Window Resized Event: " << *e << std::endl;

  if ((e->get_width() == 0) || (e->get_height() == 0)) {
    m_minimized_ = true;
    return false;
  }

  m_minimized_ = false;

  // Notify Renderer
  Renderer::Renderer::on_window_resize(e->get_width(), e->get_height());

  return false;
}

}  // namespace xuzy
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

void WindowApp::launch_tasks() {}

void WindowApp::main_loop() {
  while (m_running_) {
    m_window_->OnUpdate();
  }
}

void WindowApp::on_event(Ref<Event> event, bool& handled) {
  handled = false;

  switch (event->GetEventId()) {
    case EventId::WindowClose:
      LOG(INFO) << "Window Close Clicked (Event: " << *event << ")"
                << std::endl;
      OnWindowClose(std::static_pointer_cast<WindowCloseEvent>(event));
      handled = true;
      break;

    default:
      LOG(INFO) << "Other Event: " << *event << std::endl;
      break;
  }
}

bool WindowApp::OnWindowClose(Ref<WindowCloseEvent> e) {
  m_running_ = false;
  return true;
}

}  // namespace xuzy
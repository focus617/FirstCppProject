#include "window/core/app_context.hpp"

namespace xuzy::Window {

AppContext::AppContext(const std::string& p_app_name,
                       const std::string& p_version)
    : m_app_name_{p_app_name}, m_version_{p_version} {
  /* Settings */
  m_props_ = WindowProps(p_app_name);

  /* Window creation */
  m_window_ = Window::AWindow::Create(m_props_);

  Ref<Window::WindowImpl> window_ptr =
      std::static_pointer_cast<Window::WindowImpl>(m_window_);

  /* Center Window */
  auto [monWidth, monHeight] = window_ptr->m_monitor.get_monitor_size();
  auto [winWidth, winHeight] = window_ptr->get_size();
  window_ptr->set_position(monWidth / 2 - winWidth / 2,
                           monHeight / 2 - winHeight / 2);
}

AppContext::~AppContext() {}

GLFWwindow* AppContext::get_native_window() const {
  return ((Window::WindowImpl&)(*m_window_)).get_native_window();
}

}  // namespace xuzy::Window
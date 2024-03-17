#include "window/backend_glfw/monitor.hpp"

namespace xuzy::Window {

Monitor::Monitor(const WindowProps& p_props)
    : m_vsync{p_props.enable_vsync}, m_refresh_rate{p_props.refresh_rate} {
  // Initialize GLFW library
  if (!m_glfw_initialized) {
    LOG(INFO) << "Initializing GLFW...";
    LOG(INFO) << "GLFW version: " << glfwGetVersionString();

    int initialization_code = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == initialization_code)
        << "Could not initialize GLFW!";

    // Error Callback needed by GLFW
    auto glfw_error_callback = [](int p_error, const char* p_description) {
      LOG(ERROR) << "GLFW Error (" << p_error << ": " << p_description << ")";
    };
    glfwSetErrorCallback(glfw_error_callback);
    m_glfw_initialized = true;
  }

  // Initialize Backend
  m_context_ = AGraphicsContext::Create(p_props);
  m_context_->init();

  glfwWindowHint(GLFW_RESIZABLE, p_props.resizable);
  glfwWindowHint(GLFW_DECORATED, p_props.decorated);
  glfwWindowHint(GLFW_FOCUSED, p_props.focused);
  glfwWindowHint(GLFW_MAXIMIZED, p_props.maximized);
  glfwWindowHint(GLFW_FLOATING, p_props.floating);
  glfwWindowHint(GLFW_VISIBLE, p_props.visible);
  glfwWindowHint(GLFW_AUTO_ICONIFY, p_props.auto_iconify);
  glfwWindowHint(GLFW_REFRESH_RATE, p_props.refresh_rate);
  glfwWindowHint(GLFW_SAMPLES, p_props.samples);
}

Monitor::~Monitor() {
  LOG(INFO) << "Terminating GLFW...";

  if (m_glfw_initialized) {
    glfwTerminate();
    m_glfw_initialized = false;
  }
}

GLFWmonitor* Monitor::get_primariy_monitor() const {
  return glfwGetPrimaryMonitor();
}

std::pair<int16_t, int16_t> Monitor::get_monitor_size() const {
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  return std::pair<int16_t, int16_t>(static_cast<int16_t>(mode->width),
                                     static_cast<int16_t>(mode->height));
}

void Monitor::setup_vsync() {
  if (m_vsync)
    glfwSwapInterval(1);  // Enable vsync
  else
    glfwSwapInterval(0);
}

void Monitor::set_vsync(bool enabled) {
  m_vsync = enabled;
  setup_vsync();
}

bool Monitor::is_vsync() const { return m_vsync; }

int32_t Monitor::get_refresh_rate() const { return m_refresh_rate; }

void Monitor::set_refresh_rate(int32_t p_refresh_rate) {
  m_refresh_rate = p_refresh_rate;
}

void Monitor::driver_info() const {
  if (nullptr != m_context_) m_context_->vendor_info();
}

}  // namespace xuzy::Window
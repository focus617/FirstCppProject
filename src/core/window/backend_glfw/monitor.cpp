#include "window/backend_glfw/monitor.hpp"

namespace xuzy::Window {

Monitor::Monitor(const WindowProps& p_props)
    : m_vsync{p_props.enable_vsync}, m_refresh_rate{p_props.refresh_rate} {
  LOG(INFO) << "GLFW version: " << glfwGetVersionString();

  m_glfw_context_.m_major_version = p_props.context_major_version;
  m_glfw_context_.m_minor_version = p_props.context_minor_version;
  m_glfw_context_.m_debug_profile = p_props.debug_profile;

  // Initialize GLFW library
  if (!m_glfw_initialized) {
    int initialization_code = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == initialization_code)
        << "Could not intialize GLFW!";

    // Error Callback needed by GLFW
    auto glfw_error_callback = [](int p_error, const char* p_description) {
      LOG(ERROR) << "GLFW Error (" << p_error << ": " << p_description << ")";
    };
    glfwSetErrorCallback(glfw_error_callback);
    m_glfw_initialized = true;
  }

  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                 static_cast<int>(m_glfw_context_.m_major_version));
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                 static_cast<int>(m_glfw_context_.m_minor_version));
  // 3.2+ only 使用核心模式
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // If OpenGL debug mode is open
  if (m_glfw_context_.m_debug_profile)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

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

}  // namespace xuzy::Window
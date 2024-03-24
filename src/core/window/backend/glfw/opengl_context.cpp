#include "window/backend/glfw/opengl_context.hpp"

#include <glad/glad.h>  // Will drag system OpenGL headers
#include <GLFW/glfw3.h>

namespace xuzy::Window {

OpenGLContext::OpenGLContext(const WindowProps& p_props)
    : m_major_version(p_props.context_major_version),
      m_minor_version(p_props.context_minor_version),
      m_debug_profile(p_props.debug_profile) {}

void OpenGLContext::init() {
  LOG(INFO) << "Config OpenGL Version: " << m_major_version << "."
            << m_minor_version;

  // Decide GL+GLSL versions
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(m_major_version));
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(m_minor_version));
  // 3.2+ only 使用核心模式
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // If OpenGL debug mode is open
  if (m_debug_profile) glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
}

void OpenGLContext::vendor_info() {
  // 为GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  XUZY_CHECK_(status) << "Failed to initialize Glad!";

  LOG(INFO) << "OpenGL Info:";
  LOG(INFO) << "    OpenGL Vendor   : " << glGetString(GL_VENDOR);
  LOG(INFO) << "    OpenGL Renderer : " << glGetString(GL_RENDERER);
  LOG(INFO) << "    OpenGL Version  : " << glGetString(GL_VERSION);
}

}  // namespace xuzy::Window
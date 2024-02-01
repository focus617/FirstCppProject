#include "pch.h"

#include "linux_window.hpp"

#include "event/application_event.hpp"

namespace xuzy {

static bool s_GLFWInitialized = false;

// Error Callback needed by GLFW
static void GLFWErrorCallback(int error, const char* description) {
  LOG(ERROR) << "GLFW Error (" << error << ": " << description << ")";
}

// class LinuxWindow
#if defined(XUZY_OS_LINUX)
Window* Window::Create(const WindowProps& props) {
  return new LinuxWindow(props);
}
#endif  // defined(XUZY_OS_LINUX)

LinuxWindow::LinuxWindow(const WindowProps& props) { Init(props); }

LinuxWindow::~LinuxWindow() { Shutdown(); }

void LinuxWindow::Shutdown() {
  glfwDestroyWindow(m_Window);
  // We may have more than one GLFW windows
  // TODO: glfwTerminate on system shutdown
}

void LinuxWindow::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

void LinuxWindow::SetVSync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);
  else
    glfwSwapInterval(0);

  m_Data.VSync = enabled;
}

bool LinuxWindow::IsVSync() const { return m_Data.VSync; }

void LinuxWindow::Init(const WindowProps& props) {
  m_Data.Title = props.Title;
  m_Data.Width = props.Width;
  m_Data.Height = props.Height;

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == success) << "Could not intialize GLFW!";
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  m_Window = glfwCreateWindow((int)props.Width, (int)props.Height,
                              m_Data.Title.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);
  glfwSetWindowUserPointer(m_Window, &m_Data);
  SetVSync(true);

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width,
                                         int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.Width = width;
    data.Height = height;

    auto event = CreateRef<WindowResizeEvent>(WindowResizeEvent(width, height));
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });

  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    auto event = CreateRef<WindowCloseEvent>(WindowCloseEvent());
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });
}

}  // namespace xuzy
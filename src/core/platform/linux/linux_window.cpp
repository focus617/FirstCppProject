#include "pch.h"

#include "linux_window.hpp"

#include <glad/glad.h>

#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"

#define GL_SILENCE_DEPRECATION

namespace xuzy {

// class LinuxWindow
#if defined(XUZY_OS_LINUX)
Window* Window::Create(const WindowProps& props) {
  return new LinuxWindow(props);
}
#endif  // defined(XUZY_OS_LINUX)

static bool s_GLFWInitialized = false;

// Error Callback needed by GLFW
static void GLFWErrorCallback(int error, const char* description) {
  LOG(ERROR) << "GLFW Error (" << error << ": " << description << ")";
}

LinuxWindow::LinuxWindow(const WindowProps& props) { Init(props); }

LinuxWindow::~LinuxWindow() { Shutdown(); }

void LinuxWindow::set_vsync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);  // Enable vsync
  else
    glfwSwapInterval(0);

  m_data_.VSync = enabled;
}

bool LinuxWindow::is_vsync() const { return m_data_.VSync; }

void LinuxWindow::Init(const WindowProps& props) {
  m_data_.Title = props.Title;
  m_data_.Width = props.Width;
  m_data_.Height = props.Height;

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == success) << "Could not intialize GLFW!";
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
  // GL 3.0 + GLSL 130
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
  // only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only
#endif

  // Create window with graphics context
  p_glfw_window_ = glfwCreateWindow((int)props.Width, (int)props.Height,
                                    m_data_.Title.c_str(), nullptr, nullptr);
  XUZY_CHECK_(nullptr != p_glfw_window_) << "Could not create GLFW Window!";
  glfwMakeContextCurrent(p_glfw_window_);

  int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  XUZY_CHECK_(status) << "Failed to initialize Glad!";

  glfwSetWindowUserPointer(p_glfw_window_, &m_data_);
  set_vsync(true);  // Enable vsync

  // Set GLFW callbacks
  glfwSetWindowSizeCallback(p_glfw_window_, [](GLFWwindow* window, int width,
                                               int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.Width = width;
    data.Height = height;

    auto event = CreateRef<WindowResizeEvent>(WindowResizeEvent(width, height));
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });

  glfwSetWindowCloseCallback(p_glfw_window_, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    auto event = CreateRef<WindowCloseEvent>(WindowCloseEvent());
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });

  glfwSetKeyCallback(p_glfw_window_, [](GLFWwindow* window, int key,
                                        int scancode, int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        auto event = CreateRef<KeyPressedEvent>(KeyPressedEvent(key, 0));
        data.eventDispatcher.publish_event(event);
        break;
      }
      case GLFW_RELEASE: {
        auto event = CreateRef<KeyReleasedEvent>(KeyReleasedEvent(key));
        data.eventDispatcher.publish_event(event);
        break;
      }
      case GLFW_REPEAT: {
        auto event = CreateRef<KeyPressedEvent>(KeyPressedEvent(key, 1));
        data.eventDispatcher.publish_event(event);
        break;
      }
    }
    data.eventDispatcher.dispatch();
  });

  glfwSetCharCallback(
      p_glfw_window_, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        auto event = CreateRef<KeyTypedEvent>(KeyTypedEvent(keycode));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });

  glfwSetMouseButtonCallback(p_glfw_window_, [](GLFWwindow* window, int button,
                                                int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    switch (action) {
      case GLFW_PRESS: {
        auto event =
            CreateRef<MouseButtonPressedEvent>(MouseButtonPressedEvent(button));
        data.eventDispatcher.publish_event(event);
        break;
      }
      case GLFW_RELEASE: {
        auto event = CreateRef<MouseButtonReleasedEvent>(
            MouseButtonReleasedEvent(button));
        data.eventDispatcher.publish_event(event);
        break;
      }
    }
    data.eventDispatcher.dispatch();
  });

  glfwSetScrollCallback(
      p_glfw_window_, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        auto event = CreateRef<MouseScrolledEvent>(
            MouseScrolledEvent((float)xOffset, (float)yOffset));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });

  glfwSetCursorPosCallback(
      p_glfw_window_, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        auto event = CreateRef<MouseMovedEvent>(
            MouseMovedEvent((float)xPos, (float)yPos));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });
}

void LinuxWindow::Shutdown() {
  glfwDestroyWindow(p_glfw_window_);
  // We may have more than one GLFW windows
  // TODO: glfwTerminate on system shutdown
}

void LinuxWindow::on_update() {
  // Poll and handle events (inputs, window resize, etc.)
  // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell
  // if dear imgui wants to use your inputs.
  // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
  // your main application, or clear/overwrite your copy of the mouse data.
  // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data
  // to your main application, or clear/overwrite your copy of the keyboard
  // data. Generally you may always pass all inputs to dear imgui, and hide them
  // from your application based on those two flags.
  glfwPollEvents();

  glfwSwapBuffers(p_glfw_window_);
}

}  // namespace xuzy
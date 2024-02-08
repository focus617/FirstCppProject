#include "pch.h"

#include "window_impl.hpp"

#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"

#define GL_SILENCE_DEPRECATION

namespace xuzy {

// class WindowImpl
#if defined(XUZY_OS_LINUX)
Window* Window::Create(const WindowProps& props) {
  return new WindowImpl(props);
}
#endif  // defined(XUZY_OS_LINUX)

static bool s_glfw_initialized = false;

// Error Callback needed by GLFW
static void glfw_error_callback(int error, const char* description) {
  LOG(ERROR) << "GLFW Error (" << error << ": " << description << ")";
}

WindowImpl::WindowImpl(const WindowProps& props) {
  LOG(INFO) << "GLFW version: " << glfwGetVersionString();

  m_data_.Title = props.Title;
  m_data_.xPos = props.xPos;
  m_data_.yPos = props.yPos;
  m_data_.Width = props.Width;
  m_data_.Height = props.Height;

  glfw_window_init();
}

WindowImpl::~WindowImpl() { glfw_window_shutdown(); }

void WindowImpl::set_vsync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);  // Enable vsync
  else
    glfwSwapInterval(0);

  m_data_.VSync = enabled;
}

bool WindowImpl::is_vsync() const { return m_data_.VSync; }

void WindowImpl::glfw_window_init() {
  if (!s_glfw_initialized) {
    int success = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == success) << "Could not intialize GLFW!";
    glfwSetErrorCallback(glfw_error_callback);
    s_glfw_initialized = true;
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
  // GL 4.6 + GLSL 460
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // 3.2+ only 使用核心模式
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 3.0+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // Create window with graphics context
  p_glfw_window_handle_ =
      glfwCreateWindow((int)m_data_.Width, (int)m_data_.Height,
                       m_data_.Title.c_str(), nullptr, nullptr);
  XUZY_CHECK_(nullptr != p_glfw_window_handle_)
      << "Could not create GLFW Window!";
  glfwMakeContextCurrent(p_glfw_window_handle_);

  // 为GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
  // int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  // XUZY_CHECK_(status) << "Failed to initialize Glad!";

  glfwSetWindowUserPointer(p_glfw_window_handle_, &m_data_);
  set_vsync(true);  // Enable vsync

  // Set GLFW callbacks
  glfwSetWindowPosCallback(
      p_glfw_window_handle_, [](GLFWwindow* window, int xpos, int ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.xPos = xpos;
        data.yPos = ypos;

        auto event = CreateRef<WindowMovedEvent>(WindowMovedEvent(xpos, ypos));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });

  glfwSetWindowSizeCallback(p_glfw_window_handle_, [](GLFWwindow* window,
                                                      int width, int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.Width = width;
    data.Height = height;

    auto event = CreateRef<WindowResizeEvent>(WindowResizeEvent(width, height));
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });

  glfwSetWindowCloseCallback(p_glfw_window_handle_, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    auto event = CreateRef<WindowCloseEvent>(WindowCloseEvent());
    data.eventDispatcher.publish_event(event);

    data.eventDispatcher.dispatch();
  });

  glfwSetKeyCallback(
      p_glfw_window_handle_,
      [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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
      p_glfw_window_handle_, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        auto event = CreateRef<KeyTypedEvent>(KeyTypedEvent(keycode));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });

  glfwSetMouseButtonCallback(
      p_glfw_window_handle_,
      [](GLFWwindow* window, int button, int action, int mods) {
        // Always forward mouse data to ImGui.
        // This should be automatic with default backends,
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        // io.AddMouseButtonEvent(button, action);

        // Only forward mouse data to my underlying app
        if (!io.WantCaptureMouse) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          switch (action) {
            case GLFW_PRESS: {
              auto event = CreateRef<MouseButtonPressedEvent>(
                  MouseButtonPressedEvent(button));
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
        }
      });

  glfwSetScrollCallback(
      p_glfw_window_handle_,
      [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        auto event = CreateRef<MouseScrolledEvent>(
            MouseScrolledEvent((float)xOffset, (float)yOffset));
        data.eventDispatcher.publish_event(event);

        data.eventDispatcher.dispatch();
      });

  glfwSetCursorPosCallback(p_glfw_window_handle_, [](GLFWwindow* window,
                                                     double xPos, double yPos) {
    // Always forward mouse data to ImGui.
    // This should be automatic with default backends,
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Only forward mouse data to my underlying app
    if (!io.WantCaptureMouse) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      auto event =
          CreateRef<MouseMovedEvent>(MouseMovedEvent((float)xPos, (float)yPos));
      data.eventDispatcher.publish_event(event);

      data.eventDispatcher.dispatch();
    }
  });
}

void WindowImpl::glfw_window_shutdown() {
  glfwDestroyWindow(p_glfw_window_handle_);
  glfwTerminate();
  // We may have more than one GLFW windows in future
  // TODO: glfwTerminate on system shutdown
}

void WindowImpl::on_update() {
  // Swap the back buffer with the front buffer
  glfwSwapBuffers(p_glfw_window_handle_);

  // Poll and handle all GLFW events (inputs, window resize, etc.)
  glfwPollEvents();
}

}  // namespace xuzy
#include "pch.h"

#include "window_impl.hpp"

#include "tools/event/application_event.hpp"
#include "tools/event/key_event.hpp"
#include "tools/event/mouse_event.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define GL_SILENCE_DEPRECATION

namespace xuzy::Window {

// class WindowImpl
#if defined(XUZY_OS_LINUX)
Window* Window::Create(const WindowProps& p_props) {
  return new WindowImpl(p_props);
}
#endif  // defined(XUZY_OS_LINUX)

static bool s_glfw_initialized = false;

WindowImpl::WindowImpl(const WindowProps& props) {
  LOG(INFO) << "GLFW version: " << glfwGetVersionString();

  m_data_.m_title = props.title;
  m_data_.m_size =
      std::pair<unsigned int, unsigned int>(props.width, props.height);
  m_data_.m_maximum_size = std::pair<unsigned int, unsigned int>(
      props.maximum_width, props.maximum_height);
  m_data_.m_minimum_size = std::pair<unsigned int, unsigned int>(
      props.minimum_width, props.minimum_height);
  m_data_.m_fullscreen = props.fullscreen;
  m_data_.m_vsync = props.enable_vsync;
  m_data_.m_refresh_rate = props.refresh_rate;
  m_data_.m_cursor_mode = props.cursor_mode;
  m_data_.m_cursor_shape = props.cursor_shape;

  m_glfw_context_.m_major_version = props.context_major_version;
  m_glfw_context_.m_minor_version = props.context_minor_version;
  m_glfw_context_.m_debug_profile = props.debug_profile;

  // Initialize GLFW library
  if (!s_glfw_initialized) {
    int success = glfwInit();
    XUZY_CHECK_(GLFW_TRUE == success) << "Could not intialize GLFW!";

    // Error Callback needed by GLFW
    auto glfw_error_callback = [](int p_error, const char* p_description) {
      LOG(ERROR) << "GLFW Error (" << p_error << ": " << p_description << ")";
    };
    glfwSetErrorCallback(glfw_error_callback);
    s_glfw_initialized = true;
  }

  /* GLFW Window initialization */
  glfw_window_init(props);

  /* GLFW Cursor initialization */
  glfw_cursors_init();

  /* Callback binding */
  glfw_setup_callback();
}

WindowImpl::~WindowImpl() {
  glfw_cursors_destroy();
  glfw_window_shutdown();
}

void WindowImpl::on_update() {
  // Swap the back buffer with the front buffer
  glfwSwapBuffers(m_glfw_window_);

  // Poll and handle all GLFW events (inputs, window resize, etc.)
  glfwPollEvents();
}

void WindowImpl::set_icon(const std::string& p_filepath) {
  GLFWimage images[1];
  images[0].pixels =
      stbi_load(p_filepath.c_str(), &images[0].width, &images[0].height, 0, 4);
  glfwSetWindowIcon(m_glfw_window_, 1, images);
}

void WindowImpl::set_icon_from_memory(uint8_t* p_data, uint32_t p_width,
                                      uint32_t p_height) {
  GLFWimage images[1];
  images[0].pixels = p_data;
  images[0].height = p_width;
  images[0].width = p_height;
  glfwSetWindowIcon(m_glfw_window_, 1, images);
}

std::string WindowImpl::get_title() const { return m_data_.m_title; }

void WindowImpl::set_title(const std::string& p_title) {
  m_data_.m_title = p_title;
  glfwSetWindowTitle(m_glfw_window_, p_title.c_str());
}

void WindowImpl::set_should_close(bool p_value) const {
  glfwSetWindowShouldClose(m_glfw_window_, p_value);
}

bool WindowImpl::should_close() const {
  return glfwWindowShouldClose(m_glfw_window_);
}

void WindowImpl::set_fullscreen(bool p_value) {
  if (p_value) m_data_.m_fullscreen = true;

  glfwSetWindowMonitor(
      m_glfw_window_, p_value ? glfwGetPrimaryMonitor() : nullptr,
      static_cast<int>(m_data_.m_position.first),
      static_cast<int>(m_data_.m_position.second),
      static_cast<int>(m_data_.m_size.first),
      static_cast<int>(m_data_.m_size.second), m_data_.m_refresh_rate);

  if (!p_value) m_data_.m_fullscreen = false;
}

void WindowImpl::toggle_fullscreen() {
  set_fullscreen(!(m_data_.m_fullscreen));
}

void WindowImpl::minimize() const { glfwIconifyWindow(m_glfw_window_); }

void WindowImpl::maximize() const { glfwMaximizeWindow(m_glfw_window_); }

void WindowImpl::restore() const { glfwRestoreWindow(m_glfw_window_); }

void WindowImpl::hide() const { glfwHideWindow(m_glfw_window_); }

void WindowImpl::show() const { glfwShowWindow(m_glfw_window_); }

void WindowImpl::focus() const { glfwFocusWindow(m_glfw_window_); }

std::pair<int16_t, int16_t> WindowImpl::get_position() const {
  int x, y;
  glfwGetWindowPos(m_glfw_window_, &x, &y);
  return std::make_pair(static_cast<int16_t>(x), static_cast<int16_t>(y));
}

void WindowImpl::set_position(int16_t p_x, int16_t p_y) {
  glfwSetWindowPos(m_glfw_window_, static_cast<int>(p_x),
                   static_cast<int>(p_y));
}

std::pair<uint16_t, uint16_t> WindowImpl::get_size() const {
  int width, height;
  glfwGetWindowSize(m_glfw_window_, &width, &height);
  return std::make_pair(static_cast<uint16_t>(width),
                        static_cast<uint16_t>(height));
}

void WindowImpl::set_size(uint16_t p_width, uint16_t p_height) {
  m_data_.m_size.first = p_width;
  m_data_.m_size.second = p_height;

  glfwSetWindowSize(m_glfw_window_, static_cast<int>(p_width),
                    static_cast<int>(p_height));
}

void WindowImpl::set_minimum_size(int16_t p_minimumWidth,
                                  int16_t p_minimumHeight) {
  m_data_.m_minimum_size.first = p_minimumWidth;
  m_data_.m_minimum_size.second = p_minimumHeight;

  glfw_update_size_limit();
}

void WindowImpl::set_maximum_size(int16_t p_maximumWidth,
                                  int16_t p_maximumHeight) {
  m_data_.m_maximum_size.first = p_maximumWidth;
  m_data_.m_maximum_size.second = p_maximumHeight;

  glfw_update_size_limit();
}

void WindowImpl::make_current_context_to_thread() const {
  glfwMakeContextCurrent(m_glfw_window_);
}

std::pair<uint16_t, uint16_t> WindowImpl::get_framebuffer_size() const {
  int width, height;
  glfwGetFramebufferSize(m_glfw_window_, &width, &height);
  return std::make_pair(static_cast<uint16_t>(width),
                        static_cast<uint16_t>(height));
}

int32_t WindowImpl::get_refreshRate() const { return m_data_.m_refresh_rate; }

void WindowImpl::set_refreshRate(int32_t p_refresh_rate) {
  m_data_.m_refresh_rate = p_refresh_rate;
}

void WindowImpl::set_vsync(bool enabled) {
  if (enabled)
    glfwSwapInterval(1);  // Enable vsync
  else
    glfwSwapInterval(0);

  m_data_.m_vsync = enabled;
}

void WindowImpl::glfw_cursors_create() {
  m_cursors_[Cursor::CursorShape::ARROW] =
      glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::ARROW));
  m_cursors_[Cursor::CursorShape::IBEAM] =
      glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::IBEAM));
  m_cursors_[Cursor::CursorShape::CROSSHAIR] = glfwCreateStandardCursor(
      static_cast<int>(Cursor::CursorShape::CROSSHAIR));
  m_cursors_[Cursor::CursorShape::HAND] =
      glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::HAND));
  m_cursors_[Cursor::CursorShape::HRESIZE] =
      glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::HRESIZE));
  m_cursors_[Cursor::CursorShape::VRESIZE] =
      glfwCreateStandardCursor(static_cast<int>(Cursor::CursorShape::VRESIZE));
}

void WindowImpl::glfw_cursors_destroy() {
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::ARROW]);
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::IBEAM]);
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::CROSSHAIR]);
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::HAND]);
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::HRESIZE]);
  glfwDestroyCursor(m_cursors_[Cursor::CursorShape::VRESIZE]);
}

GLFWcursor* WindowImpl::get_cursor_instance(
    Cursor::CursorShape p_cursorShape) const {
  return m_cursors_.at(p_cursorShape);
}

void WindowImpl::set_cursor_mode(Cursor::CursorMode p_cursorMode) {
  m_data_.m_cursor_mode = p_cursorMode;
  glfwSetInputMode(m_glfw_window_, GLFW_CURSOR, static_cast<int>(p_cursorMode));
}

void WindowImpl::set_cursor_shape(Cursor::CursorShape p_cursorShape) {
  m_data_.m_cursor_shape = p_cursorShape;
  glfwSetCursor(m_glfw_window_, get_cursor_instance(p_cursorShape));
}

void WindowImpl::set_cursor_position(int16_t p_x, int16_t p_y) {
  glfwSetCursorPos(m_glfw_window_, static_cast<double>(p_x),
                   static_cast<double>(p_y));
}

bool WindowImpl::is_vsync() const { return m_data_.m_vsync; }

bool WindowImpl::is_resizable() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_RESIZABLE) == GLFW_TRUE;
}

bool WindowImpl::is_fullscreen() const { return m_data_.m_fullscreen; }

bool WindowImpl::is_hidden() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_VISIBLE) == GLFW_FALSE;
}

bool WindowImpl::is_visible() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_VISIBLE) == GLFW_TRUE;
}

bool WindowImpl::is_maximized() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_MAXIMIZED) == GLFW_TRUE;
}

bool WindowImpl::is_minimized() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_MAXIMIZED) == GLFW_FALSE;
}

bool WindowImpl::is_focused() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_FOCUSED) == GLFW_TRUE;
}

bool WindowImpl::is_decorated() const {
  return glfwGetWindowAttrib(m_glfw_window_, GLFW_DECORATED) == GLFW_TRUE;
  ;
}

void WindowImpl::glfw_window_init(const WindowProps& props) {
  GLFWmonitor* selected_monitor = nullptr;
  // 如果要创造全屏窗口，就需要一个控制器
  if (m_data_.m_fullscreen) selected_monitor = glfwGetPrimaryMonitor();

  glfwWindowHint(GLFW_RESIZABLE, props.resizable);
  glfwWindowHint(GLFW_DECORATED, props.decorated);
  glfwWindowHint(GLFW_FOCUSED, props.focused);
  glfwWindowHint(GLFW_MAXIMIZED, props.maximized);
  glfwWindowHint(GLFW_FLOATING, props.floating);
  glfwWindowHint(GLFW_VISIBLE, props.visible);
  glfwWindowHint(GLFW_AUTO_ICONIFY, props.auto_iconify);
  glfwWindowHint(GLFW_REFRESH_RATE, props.refresh_rate);
  glfwWindowHint(GLFW_SAMPLES, props.samples);

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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,
                 static_cast<int>(m_glfw_context_.m_major_version));
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,
                 static_cast<int>(m_glfw_context_.m_minor_version));
  // 3.2+ only 使用核心模式
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  // If OpenGL debug mode is open
  if (m_glfw_context_.m_debug_profile)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

  // Create window with graphics context
  m_glfw_window_ =
      glfwCreateWindow(static_cast<int>(m_data_.m_size.first),
                       static_cast<int>(m_data_.m_size.second),
                       m_data_.m_title.c_str(), selected_monitor, nullptr);
  XUZY_CHECK_(nullptr != m_glfw_window_) << "Could not create GLFW Window!";
  glfwMakeContextCurrent(m_glfw_window_);
  glfw_update_size_limit();

  auto [x, y] = get_position();
  m_data_.m_position.first = x;
  m_data_.m_position.second = y;

  // 为GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
  // int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  // XUZY_CHECK_(status) << "Failed to initialize Glad!";

  glfwSetWindowUserPointer(m_glfw_window_, &m_data_);
  set_vsync(m_data_.m_vsync);  // Enable vsync
}

void WindowImpl::glfw_window_shutdown() {
  glfwDestroyWindow(m_glfw_window_);
  glfwTerminate();
  // We may have more than one GLFW windows in future
  // TODO: glfwTerminate on system shutdown
}

void WindowImpl::glfw_cursors_init() {
  glfw_cursors_create();
  set_cursor_mode(m_data_.m_cursor_mode);
  set_cursor_shape(m_data_.m_cursor_shape);
}

void WindowImpl::glfw_setup_callback() {
  // Set GLFW callbacks
  glfwSetWindowPosCallback(
      m_glfw_window_, [](GLFWwindow* window, int xpos, int ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.m_position.first = xpos;
        data.m_position.second = ypos;
        // Produce event
        auto event = CreateRef<WindowMovedEvent>(WindowMovedEvent(xpos, ypos));
        // Publish to application
        data.eventDispatcher.dispatch(event);
      });

  glfwSetWindowSizeCallback(m_glfw_window_, [](GLFWwindow* window, int width,
                                               int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.m_size.first = width;
    data.m_size.second = height;
    // Produce event
    auto event = CreateRef<WindowResizeEvent>(WindowResizeEvent(width, height));
    // Publish to application
    data.eventDispatcher.dispatch(event);
  });

  glfwSetWindowCloseCallback(m_glfw_window_, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

    auto event = CreateRef<WindowCloseEvent>(WindowCloseEvent());
    // Publish to application
    data.eventDispatcher.dispatch(event);
  });

  glfwSetKeyCallback(m_glfw_window_, [](GLFWwindow* window, int key,
                                        int scancode, int action, int mods) {
    // Always forward keyboard data to ImGui.
    // This should be automatic with default backends,
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Only forward keyboard data to my underlying app
    if (!io.WantCaptureKeyboard) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      switch (action) {
        case GLFW_PRESS: {
          auto event = CreateRef<KeyPressedEvent>(KeyPressedEvent(key, 0));
          // Publish to application
          data.eventDispatcher.dispatch(event);
          break;
        }
        case GLFW_RELEASE: {
          auto event = CreateRef<KeyReleasedEvent>(KeyReleasedEvent(key));
          // Publish to application
          data.eventDispatcher.dispatch(event);
          break;
        }
        case GLFW_REPEAT: {
          auto event = CreateRef<KeyPressedEvent>(KeyPressedEvent(key, 1));
          // Publish to application
          data.eventDispatcher.dispatch(event);
          break;
        }
      }
    }
  });

  glfwSetCharCallback(
      m_glfw_window_, [](GLFWwindow* window, unsigned int keycode) {
        // Always forward keyboard data to ImGui.
        // This should be automatic with default backends,
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        // Only forward keyboard data to my underlying app
        if (!io.WantCaptureKeyboard) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          auto event = CreateRef<KeyTypedEvent>(KeyTypedEvent(keycode));
          // Publish to application
          data.eventDispatcher.dispatch(event);
        }
      });

  glfwSetMouseButtonCallback(
      m_glfw_window_, [](GLFWwindow* window, int button, int action, int mods) {
        // Always forward mouse data to ImGui.
        // This should be automatic with default backends,
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        // Only forward mouse data to my underlying app
        if (!io.WantCaptureMouse) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          switch (action) {
            case GLFW_PRESS: {
              auto event = CreateRef<MouseButtonPressedEvent>(
                  MouseButtonPressedEvent(button));
              // Publish to application
              data.eventDispatcher.dispatch(event);
              break;
            }
            case GLFW_RELEASE: {
              auto event = CreateRef<MouseButtonReleasedEvent>(
                  MouseButtonReleasedEvent(button));
              // Publish to application
              data.eventDispatcher.dispatch(event);
              break;
            }
          }
        }
      });

  glfwSetScrollCallback(
      m_glfw_window_, [](GLFWwindow* window, double xOffset, double yOffset) {
        // Always forward mouse data to ImGui.
        // This should be automatic with default backends,
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        // Only forward mouse data to my underlying app
        if (!io.WantCaptureMouse) {
          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

          auto event = CreateRef<MouseScrolledEvent>(
              MouseScrolledEvent((float)xOffset, (float)yOffset));
          // Publish to application
          data.eventDispatcher.dispatch(event);
        }
      });

  glfwSetCursorPosCallback(m_glfw_window_, [](GLFWwindow* window, double xPos,
                                              double yPos) {
    // Always forward mouse data to ImGui.
    // This should be automatic with default backends,
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Only forward mouse data to my underlying app
    if (!io.WantCaptureMouse) {
      WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

      auto event =
          CreateRef<MouseMovedEvent>(MouseMovedEvent((float)xPos, (float)yPos));
      // Publish to application
      data.eventDispatcher.dispatch(event);
    }
  });
}

void WindowImpl::glfw_update_size_limit() const {
  glfwSetWindowSizeLimits(m_glfw_window_,
                          static_cast<int>(m_data_.m_minimum_size.first),
                          static_cast<int>(m_data_.m_minimum_size.second),
                          static_cast<int>(m_data_.m_maximum_size.first),
                          static_cast<int>(m_data_.m_maximum_size.second));
}

}  // namespace xuzy::Window
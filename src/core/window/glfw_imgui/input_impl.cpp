#include "pch.h"

#include "input_impl.hpp"

#include "window/core/window_app.hpp"
#include "window_impl.hpp"

namespace xuzy {

Input* Input::s_instance_ = new InputImpl();

GLFWwindow* InputImpl::get_window() {
  WindowApp& app = (WindowApp&)(App::get());
  Window::WindowImpl& win = (Window::WindowImpl&)(app.get_window());
  return static_cast<GLFWwindow*>(win.get_native_window());
}

bool InputImpl::is_key_pressed_impl(KeyCode key) {
  auto window = get_window();
  auto state = glfwGetKey(window, static_cast<int32_t>(key));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputImpl::is_mouse_button_pressed_impl(MouseCode button) {
  auto window = get_window();
  auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
  return state == GLFW_PRESS;
}

std::pair<float, float> InputImpl::get_mouse_position_impl() {
  auto window = get_window();
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);

  return {(float)xpos, (float)ypos};
}

float InputImpl::get_mouse_x_impl() {
  auto [x, y] = get_mouse_position_impl();
  return x;
}

float InputImpl::get_mouse_y_impl() {
  auto [x, y] = get_mouse_position_impl();
  return x;
}

}  // namespace xuzy
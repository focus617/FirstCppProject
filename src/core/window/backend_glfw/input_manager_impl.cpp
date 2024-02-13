#include "pch.h"

#include "input_manager_impl.hpp"

#include "window/core/window_app.hpp"
#include "window_impl.hpp"

namespace xuzy::Window::Inputs {

InputManager* InputManager::s_instance_ = new InputManagerImpl();

GLFWwindow* InputManagerImpl::get_window() {
  WindowApp& app = (WindowApp&)(App::get());
  WindowImpl& win = (WindowImpl&)(app.get_window());
  return static_cast<GLFWwindow*>(win.get_native_window());
}

Key::KeyState InputManagerImpl::get_key_state_impl(KeyCode p_key) {
  auto window = get_window();
  switch (glfwGetKey(window, static_cast<int32_t>(p_key))) {
    case GLFW_PRESS:
      return Key::KeyState::KEY_PRESS;
    case GLFW_RELEASE:
      return Key::KeyState::KEY_RELEASE;
  }
  return Key::KeyState::KEY_RELEASE;
}

Mouse::MouseButtonState InputManagerImpl::get_mouse_button_state_impl(
    MouseCode p_button) {
  auto window = get_window();
  switch (glfwGetMouseButton(window, static_cast<int32_t>(p_button))) {
    case GLFW_PRESS:
      return Mouse::MouseButtonState::MOUSE_PRESS;
    case GLFW_RELEASE:
      return Mouse::MouseButtonState::MOUSE_RELEASE;
  }
  return Mouse::MouseButtonState::MOUSE_RELEASE;
}

bool InputManagerImpl::is_key_pressed_impl(KeyCode p_key) {
  auto window = get_window();
  auto state = glfwGetKey(window, static_cast<int32_t>(p_key));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputManagerImpl::is_mouse_button_pressed_impl(MouseCode p_button) {
  auto window = get_window();
  auto state = glfwGetMouseButton(window, static_cast<int32_t>(p_button));
  return state == GLFW_PRESS;
}

std::pair<double, double> InputManagerImpl::get_mouse_position_impl() {
	std::pair<double, double> result;
	glfwGetCursorPos(get_window(), &result.first, &result.second);
	return result;
}

double InputManagerImpl::get_mouse_x_impl() {
  auto [x, y] = get_mouse_position_impl();
  return x;
}

double InputManagerImpl::get_mouse_y_impl() {
  auto [x, y] = get_mouse_position_impl();
  return x;
}

}  // namespace xuzy::Window::Inputs
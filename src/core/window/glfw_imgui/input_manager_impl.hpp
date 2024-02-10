#pragma once

#include <GLFW/glfw3.h>

#include "window/core/inputs/input_manager.hpp"

namespace xuzy::Window::Inputs {

class InputManagerImpl : public InputManager {
 protected:
  bool is_key_pressed_impl(KeyCode key) override;

  bool is_mouse_button_pressed_impl(MouseCode button) override;
  std::pair<float, float> get_mouse_position_impl() override;
  float get_mouse_x_impl() override;
  float get_mouse_y_impl() override;

 private:
  GLFWwindow* get_window();
};

}  // namespace xuzy::Window::Inputs
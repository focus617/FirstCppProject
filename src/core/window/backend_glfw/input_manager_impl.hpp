#pragma once

#include <GLFW/glfw3.h>

#include "window/core/inputs/input_manager.hpp"

namespace xuzy::Window::Inputs {

class InputManagerImpl : public InputManager {
 protected:
  virtual Key::KeyState get_key_state_impl(KeyCode p_key) override;

  virtual Mouse::MouseButtonState get_mouse_button_state_impl(
      MouseCode p_button) override;

  virtual bool is_key_pressed_impl(KeyCode p_key) override;

  virtual bool is_mouse_button_pressed_impl(MouseCode p_button) override;

  virtual std::pair<double, double> get_mouse_position_impl() override;
  virtual double get_mouse_x_impl() override;
  virtual double get_mouse_y_impl() override;

 private:
  GLFWwindow* get_window();
};

}  // namespace xuzy::Window::Inputs
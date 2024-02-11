#pragma once

#include "key_codes.h"
#include "key_state.h"
#include "mouse_codes.h"
#include "mouse_state.h"

namespace xuzy::Window::Inputs {

/**
 * @brief The InputManager is global in system, which enable customer to poll
 * and catch current p_key and mouse status
 */
class XUZY_API InputManager {
 public:
  virtual ~InputManager() {}

  /**
   * Return the current state of the given key
   * @param p_key
   */
  Key::KeyState get_key_state(KeyCode p_key) const {
    return s_instance_->get_key_state(p_key);
  }

  /**
   * Return the current state of the given mouse button
   * @param p_button
   */
  Mouse::MouseButtonState get_mouse_button_state(MouseCode p_button) const {
    return s_instance_->get_mouse_button_state(p_button);
  }
  /**
   * @brief Return true if the given p_key has been pressed during the frame
   * @param p_key KeyCode for keyboard input
   */
  static bool is_key_pressed(KeyCode p_key) {
    return s_instance_->is_key_pressed_impl(p_key);
  }

  /**
   * @brief Return true if the given mouse p_button has been pressed during the
   * frame
   * @param p_button MouseCode for mouse button input
   */
  static bool is_mouse_button_pressed(MouseCode p_button) {
    return s_instance_->is_mouse_button_pressed_impl(p_button);
  }

  /**
   * @brief Return the current mouse position relative to the window
   */
  static std::pair<double, double> get_mouse_position() {
    return s_instance_->get_mouse_position_impl();
  }

  /**
   * @brief Return the current mouse X position relative to the window
   */
  static double get_mouse_x() { return s_instance_->get_mouse_x_impl(); }

  /**
   * @brief Return the current mouse Y position relative to the window
   */
  static double get_mouse_y() { return s_instance_->get_mouse_y_impl(); }

 protected:
  virtual Key::KeyState get_key_state_impl(KeyCode p_key) = 0;
  virtual Mouse::MouseButtonState get_mouse_button_state_impl(
      MouseCode p_button) = 0;
  virtual bool is_key_pressed_impl(KeyCode p_key) = 0;
  virtual bool is_mouse_button_pressed_impl(MouseCode p_button) = 0;
  virtual std::pair<double, double> get_mouse_position_impl() = 0;
  virtual double get_mouse_x_impl() = 0;
  virtual double get_mouse_y_impl() = 0;

 private:
  static InputManager* s_instance_;
};

}  // namespace xuzy::Window::Inputs
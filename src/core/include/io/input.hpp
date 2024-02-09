#pragma once

#include "key_codes.hpp"
#include "mouse_codes.hpp"

namespace xuzy {

/**
 * @brief Input class is global input manager, which enable customer to poll and
 * catch current key and mouse status
 */
class XUZY_API Input {
 public:
  virtual ~Input() {}

  static bool is_key_pressed(KeyCode key) {
    return s_instance_->is_key_pressed_impl(key);
  }

  static bool is_mouse_button_pressed(MouseCode button) {
    return s_instance_->is_mouse_button_pressed_impl(button);
  }
  static std::pair<float, float> get_mouse_position() {
    return s_instance_->get_mouse_position_impl();
  }
  static float get_mouse_x() { return s_instance_->get_mouse_x_impl(); }
  static float get_mouse_y() { return s_instance_->get_mouse_y_impl(); }

 protected:
  virtual bool is_key_pressed_impl(KeyCode key) = 0;

  virtual bool is_mouse_button_pressed_impl(MouseCode button) = 0;
  virtual std::pair<float, float> get_mouse_position_impl() = 0;
  virtual float get_mouse_x_impl() = 0;
  virtual float get_mouse_y_impl() = 0;

 private:
  static Input* s_instance_;
};

}  // namespace xuzy
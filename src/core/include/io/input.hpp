#pragma once

#include "core.h"

namespace xuzy {

/**
 * @brief Input class is global input manager, which enable customer to poll and
 * catch current key and mouse status
 */
class XUZY_API Input {
 public:
  virtual ~Input() {}

  inline static bool is_key_pressed(int keycode) {
    return s_instance_->is_key_pressed_impl(keycode);
  }

  inline static bool is_mouse_button_pressed(int button) {
    return s_instance_->is_mouse_button_pressed_impl(button);
  }
  inline static std::pair<float, float> get_mouse_position() {
    return s_instance_->get_mouse_position_impl();
  }
  inline static float get_mouse_x() { return s_instance_->get_mouse_x_impl(); }
  inline static float get_mouse_y() { return s_instance_->get_mouse_y_impl(); }

 protected:
  virtual bool is_key_pressed_impl(int keycode) = 0;

  virtual bool is_mouse_button_pressed_impl(int button) = 0;
  virtual std::pair<float, float> get_mouse_position_impl() = 0;
  virtual float get_mouse_x_impl() = 0;
  virtual float get_mouse_y_impl() = 0;

 private:
  static Input* s_instance_;
};

}  // namespace xuzy
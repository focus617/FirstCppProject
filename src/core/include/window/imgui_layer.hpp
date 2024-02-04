#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"
#include "window/layer.hpp"

namespace xuzy {

class XUZY_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  void on_attach();
  void on_detach();
  void on_update();
  void on_event(Ref<Event> event, bool& handled);

 private:
  bool on_mouse_button_pressed_event(Ref<MouseButtonPressedEvent> e);
  bool on_mouse_button_released_event(Ref<MouseButtonReleasedEvent> e);
  bool on_mouse_moved_event(Ref<MouseMovedEvent> e);
  bool on_mouse_scrolled_event(Ref<MouseScrolledEvent> e);
  bool on_key_pressed_event(Ref<KeyPressedEvent> e);
  bool on_key_released_event(Ref<KeyReleasedEvent> e);
  bool on_key_typed_event(Ref<KeyTypedEvent> e);
  bool on_window_resize_event(Ref<WindowResizeEvent> e);

  void imgui_init();
  void imgui_load_fonts();
  void imgui_rendering();

  void draw();

 private:
  float m_Time = 0.0f;
};

}  // namespace xuzy
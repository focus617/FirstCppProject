#pragma once

#include "imgui.h"
#include "tools/event/application_event.hpp"
#include "tools/event/key_event.hpp"
#include "tools/event/mouse_event.hpp"
#include "window/core/layer.hpp"

namespace xuzy::Window {

class XUZY_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();

  virtual void on_attach() override;
  virtual void on_detach() override;
  virtual void on_update() override;
  virtual void on_imgui_render() override;
  virtual void on_event(Ref<Event> event, bool& handled) override;

  void begin_render();
  void end_render();

  void set_dark_theme_colors();
  void show_app_main_menubar();

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

  void imgui_toolbar();

 private:
  float m_time_ = 0.0f;

  ImVec4 m_clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  std::map<std::string, ImFont*> m_fonts_;
  float m_font_size_pixels_ = 18.0f;

  bool show_app_main_menu_bar = true;
  bool show_app_toolbar = true;
};

}  // namespace xuzy::Window
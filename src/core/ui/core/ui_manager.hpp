#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "tools/event/application_event.hpp"
#include "tools/event/key_event.hpp"
#include "tools/event/mouse_event.hpp"
#include "ui/imgui/imgui.h"
#include "ui/core/canvas.hpp"
#include "ui/style/style.h"
#include "window/core/layer.hpp"

namespace xuzy::UI {

class XUZY_API UIManager : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Create the UI manager. Will setup ImGui internally\
   * @param p_glfwWindow
   * @param p_style
   * @param p_glslVersion (Ex: #version 150)
   */
  UIManager(GLFWwindow* p_glfwWindow,
            const std::string& p_glslVersion = "#version 150",
            UI::Style p_style = UI::Style::IMGUI_DARK_STYLE);

  /**
   * @brief Destroy the UI manager. Will handle ImGui destruction internally
   */
  ~UIManager();

  /**
   * @brief Create the ImGui UI manager.
   * It will setup ImGui internally
   */
  virtual void on_attach() override;

  /**
   * @brief Destroy the ImGui UI manager.
   * It will handle ImGui destruction internally
   */
  virtual void on_detach() override;

  virtual void on_draw() override;
  virtual void on_update() override;
  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

  void begin_render();
  void end_render();

 private:
  bool on_mouse_button_pressed_event(Ref<Events::MouseButtonPressedEvent> e);
  bool on_mouse_button_released_event(Ref<Events::MouseButtonReleasedEvent> e);
  bool on_mouse_moved_event(Ref<Events::MouseMovedEvent> e);
  bool on_mouse_scrolled_event(Ref<Events::MouseScrolledEvent> e);
  bool on_key_pressed_event(Ref<Events::KeyPressedEvent> e);
  bool on_key_released_event(Ref<Events::KeyReleasedEvent> e);
  bool on_key_typed_event(Ref<Events::KeyTypedEvent> e);
  bool on_window_resize_event(Ref<Events::WindowResizeEvent> e);

  /**
   * @brief Load imgui and setup context
   */
  void imgui_init();
  void imgui_toolbar();

  /**
   * @brief Load a font
   * @returns true on success)
   */
  bool load_fonts();

  /**
   * @brief Load a font (Returns true on success)
   * @param p_id
   * @param p_path
   * @param p_font_size
   */
  bool load_font(const std::string& p_id, const std::string& p_path,
                 float p_font_size);

  /**
   * @brief Unload a font (Returns true on success)
   * @param p_id
   */
  bool unload_font(const std::string& p_id);

  /**
   * @brief Set the given font as the current one (Returns true on success)
   */
  bool use_font(const std::string& p_id);

  /**
   * @brief Use the default font (ImGui default font)
   */
  void use_default_font();

  /**
   * @brief Apply a new style to the UI elements
   * @param p_style
   */
  void apply_style(UI::Style p_style);

  /**
   * @brief Enable the docking system
   * @param p_value
   */
  void enable_docking(bool p_value);

  /**
   * @brief Return true if the docking system is enabled
   */
  bool is_docking_enabled() const;

  /**
   * @brief Allow the user to enable/disable .ini generation to
   * store layout config
   * @param p_value
   */
  void enable_layout_save(bool p_value);

  /**
   *  @brief Return true if the layout config save system is on
   */
  bool is_layout_save_enabled() const;

  /**
   * @brief Defines a filename for the layout config file
   */
  void set_layout_config_filename(const std::string& p_filename);

  /**
   * @brief Defines a frequency (in seconds) for the auto saving system
   * of the editor layout
   * @param p_frequency
   */
  void set_layout_auto_save_frequency(float p_frequency);

  /**
   * @brief Returns the current frequency (in seconds) for the auto saving
   * system of the editor layout
   */
  float get_layout_auto_save_frequency(float p_frequeny);

 private:
  std::string m_layout_config_filename = "imgui.ini";

  GLFWwindow* m_glfw_window_;
  std::string m_glsl_version_;
  UI::Style m_style_;

  UI::Canvas m_canvas_;

  std::unordered_map<std::string, ImFont*> m_fonts_;
  float m_font_size_pixels_ = 18.0f;

  ImVec4 m_clear_color_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  bool m_enable_docking = false;

  float m_time_ = 0.0f;
};

}  // namespace xuzy::UI
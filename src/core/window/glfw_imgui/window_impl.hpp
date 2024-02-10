#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "imgui.h"
#include "window/backend/opengl/imgui_impl_opengl3.h"
#include "window/core/cursor/cursor_mode.h"
#include "window/core/cursor/cursor_shape.h"
#include "window/core/window.hpp"
#include "window/glfw_imgui/imgui_impl_glfw.h"

namespace xuzy::Window {

class XUZY_API WindowImpl : public Window {
  struct WindowData;

 public:
  /**
   * @brief Create the window
   * @param p_props Window properties
   */
  WindowImpl(const WindowProps& p_props);

  /**
   * @brief Destructor of the window, responsible of the GLFW window memory free
   */
  virtual ~WindowImpl();

  /**
   * @brief Set Icon
   * @param p_filePath
   */
  void set_icon(const std::string& p_filePath);

  /**
   * @brief Set Icon from memory
   * @param p_data
   * @param p_width
   * @param p_height
   */
  void set_icon_from_memory(uint8_t* p_data, uint32_t p_width,
                            uint32_t p_height);

  /**
   * @brief Return the title of the window
   */
  std::string get_title() const;

  /**
   * @brief Define a title for the window
   * @param p_title
   */
  void set_title(const std::string& p_title);

  /**
   * @brief Set the window in fullscreen or windowed mode
   * @param p_value (True for fullscreen mode, false for windowed)
   */
  void set_fullscreen(bool p_value);

  /**
   * @brief Switch the window to fullscreen or windowed mode depending
   * on the current state
   */
  void toggle_fullscreen();

  /**
   * @brief Define a position for the window
   * @param p_x
   * @param p_y
   */
  void set_position(int16_t p_x, int16_t p_y);

  /**
   * @brief Return the current size of the window
   */
  virtual std::pair<uint16_t, uint16_t> get_size() const override;

  /**
   * @brief Resize the window
   * @param p_width
   * @param p_height
   */
  void set_size(uint16_t p_width, uint16_t p_height);

  /**
   * @brief Defines a minimum size for the window
   * @param p_minimumWidth
   * @param p_minimumHeight
   * @note -1 (WindowSettings::DontCare) value means no limitation
   */
  void set_minimum_size(int16_t p_minimumWidth, int16_t p_minimumHeight);

  /**
   * @brief Defines a maximum size for the window
   * @param p_maximumWidth
   * @param p_maximumHeight
   * @note -1 (WindowSettings::DontCare) value means no limitation
   */
  void set_maximum_size(int16_t p_maximumWidth, int16_t p_maximumHeight);

  /**
   * @brief Minimize the window
   */
  void minimize() const;

  /**
   * @brief Maximize the window
   */
  void maximize() const;

  /**
   * @brief Restore the window
   */
  void restore() const;

  /**
   * @brief Hides the specified window if it was previously visible
   */
  void hide() const;

  /**
   * @brief Show the specified window if it was previously hidden
   */
  void show() const;

  /**
   * @brief Focus the window
   */
  void focus() const;

  /**
   * @brief Return true if the windows is resizable
   */
  bool is_resizable() const;

  /**
   * @brief Return true if the window is fullscreen
   */
  bool is_fullscreen() const;

  /**
   * @brief Return true if the window is hidden
   */
  bool is_Hidden() const;

  /**
   * @brief Return true if the window is visible
   */
  bool is_visible() const;

  /**
   * @brief Return true if the windows is maximized
   */
  bool is_maximized() const;

  /**
   * @brief Return true if the windows is minimized
   */
  bool is_minimized() const;

  /**
   * @brief Return true if the windows is focused
   */
  bool is_focused() const;

  /**
   * @brief Return true if the windows is decorated
   */
  bool is_decorated() const;

  /**
   * @brief Return the current refresh rate (Only applied to the fullscreen
   * mode). If the value is -1 (WindowSettings::DontCare) the highest refresh
   * rate will be used
   */
  int32_t get_refreshRate() const;

  /**
   * @brief Defines a refresh rate (Use WindowSettings::DontCare to use the
   * highest available refresh rate)
   * @param p_refresh_rate
   * @note You need to switch to fullscreen mode to apply this effect (Or leave
   * fullscreen and re-apply)
   */
  void set_refreshRate(int32_t p_refresh_rate);

  /**
   * @brief Return an instance of GLFWcursor corresponding to the given shape
   * @param p_cursorShape
   */
  GLFWcursor* get_cursor_instance(Cursor::CursorShape p_cursorShape) const;

  /**
   * @brief Define a mode for the mouse cursor
   * @param p_cursorMode
   */
  void set_cursor_mode(Cursor::CursorMode p_cursorMode);

  /**
   * @brief Define a shape to apply to the current cursor
   * @param p_cursorShape
   */
  void set_cursor_shape(Cursor::CursorShape p_cursorShape);

  /**
   * @brief Move the cursor to the given position
   */
  void set_cursor_position(int16_t p_x, int16_t p_y);

  // Window attributes
  virtual void set_vsync(bool enabled) override;
  virtual bool is_vsync() const override;

  virtual void on_update() override;

  inline virtual void set_event_callback(
      const EventCallbackFn& callback) override {
    m_data_.eventDispatcher += callback;
  }

  /**
   * @brief Return built-in GLFWwindow
   */
  inline void* get_native_window() const { return m_glfw_window_; }

 private:
  /* GLFW Window initialization */
  void glfw_window_init();
  /* GLFW Window cleanup */
  void glfw_window_shutdown();
  /* GLFW event callback initialization */
  void glfw_setup_callback();
  /* GLFW Cursor initialization */
  void glfw_cursor_init();
  
  void glfw_update_size_limit() const;

  void glfw_create_cursors();
  void glfw_destroy_cursors();

 private:
  GLFWwindow* m_glfw_window_ = nullptr;
  std::unordered_map<Cursor::CursorShape, GLFWcursor*> m_cursors_;

  /* Window settings */
  struct WindowData {
    std::string m_title;
    std::pair<uint16_t, uint16_t> m_position;
    std::pair<uint16_t, uint16_t> m_size;
    std::pair<int16_t, int16_t> m_minimum_size;
    std::pair<int16_t, int16_t> m_maximum_size;
    bool m_fullscreen;
    int32_t m_refresh_rate;
    bool VSync;
    Cursor::CursorMode m_cursor_mode;
    Cursor::CursorShape m_cursor_shape;

    xuzy::EventDispatcher<void()> eventDispatcher;
  };

  WindowData m_data_;
};

}  // namespace xuzy::Window
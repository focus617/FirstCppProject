#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include "window/core/window_props.hpp"
#include "window/core/AGraphics_context.hpp"

namespace xuzy::Window {

/**
 * @brief The Monitor Device represents the windowing context.
 * It is necessary to create a monitor device to create a window
 */
class Monitor {
 public:
  /**
   * @brief The constructor of the monitor.
   * It will take care about GLFW initialization
   * @param p_props Window properties
   */
  Monitor(const WindowProps& p_props);

  /**
   * @brief The destructor of the monitor.
   * It will take care about GLFW destruction
   */
  ~Monitor();

  /**
   * @brief Return primary monitor
   */
  GLFWmonitor* get_primariy_monitor() const;

  /**
   * @brief Return the size, in pixels, of the primary monity
   */
  std::pair<int16_t, int16_t> get_monitor_size() const;

  /**
   * @brief Return the current refresh rate (Only applied to the fullscreen
   * mode). If the value is -1 (WindowSettings::DontCare) the highest refresh
   * rate will be used
   */
  int32_t get_refresh_rate() const;

  /**
   * @brief Defines a refresh rate (Use WindowSettings::DontCare to use the
   * highest available refresh rate)
   * @param p_refresh_rate
   * @note You need to switch to fullscreen mode to apply this effect (Or leave
   * fullscreen and re-apply)
   */
  void set_refresh_rate(int32_t p_refresh_rate);

  /**
   * @brief Enable or disable the vsync of monitor
   * @note You must call this method after creating and defining a window as the
   * current context
   */
  void setup_vsync();

  /**
   * @brief Enable or disable the vsync
   * @note You must call this method after creating and defining a window as the
   * current context
   * @param p_value (True to enable vsync)
   */
  void set_vsync(bool enabled);

  /**
   * @brief Return true if the vsync is currently enabled
   */
  bool is_vsync() const;

  /**
   * @brief Dump driver(eg. OpenGL) vendor and version
   */
  void driver_info() const;

 private:
  bool m_glfw_initialized = false;
  bool m_vsync;
  int32_t m_refresh_rate;

  /* GLFW-OpenGL Context */
  Scope<AGraphicsContext> m_context_;
};

}  // namespace xuzy::Window
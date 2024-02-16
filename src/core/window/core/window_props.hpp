
#pragma once

#include <string>

#include "window/cursor/cursor_mode.h"
#include "window/cursor/cursor_shape.h"

namespace xuzy::Window {

/**
 * @brief contains window properties
 */
struct WindowProps {
  /**
   * @brief A simple constant used to ignore a value setting (Let the program
   * decide for you)
   * @note You can you WindowSettings::DontCare only where it is indicated
   */
  static const int32_t DontCare = -1;

  /**
   * @brief Title of the window (Displayed in the title bar)
   */
  std::string title;

  /**
   * @brief Width in pixels of the window
   */
  uint16_t width;

  /**
   * @brief Height in pixels of the window
   */
  uint16_t height;

  /**
   * @brief Minimum width of the window.
   * Use WindowSettings::DontCare to disable limit
   */
  int16_t minimum_width = DontCare;

  /**
   * @brief Minimum height of the window.
   * Use WindowSettings::DontCare to disable limit
   */
  int16_t minimum_height = DontCare;

  /**
   * @brief Maximum width of the window.
   * Use WindowSettings::DontCare to disable limit
   */
  int16_t maximum_width = DontCare;

  /**
   * @brief Maximum height of the window.
   * Use WindowSettings::DontCare to disable limit
   */
  int16_t maximum_height = DontCare;

  /**
   * @brief Specifies if the window is by default in fullscreen or windowed mode
   */
  bool fullscreen = false;

  /**
   * @brief specifies whether the windowed mode window will be resizable by the
   * user. The window will still be resizable using the "SetSize(uint16_t,
   * uint16_t)" method of the "Window" class. This hint is ignored for full
   * screen windows
   */
  bool resizable = true;

  /**
   * @brief Specifies whether the windowed mode window will have window
   * decorations such as a border, a close widget, etc.
   * An undecorated window may still allow the user to generate close events on
   * some platforms. This hint is ignored for full screen windows.
   */
  bool decorated = true;

  /**
   * Specifies whether the windowed mode window will be given input focus when
   * created. This hint is ignored for full screen and initially hidden windows.
   */
  bool focused = true;

  /**
   * Specifies whether the windowed mode window will be maximized when created.
   * This hint is ignored for full screen windows.
   */
  bool maximized = false;

  /**
   * Specifies whether the windowed mode window will be floating above other
   * regular windows, also called topmost or always-on-top. This is intended
   * primarily for debugging purposes and cannot be used to implement proper
   * full screen windows. This hint is ignored for full screen windows.
   */
  bool floating = false;

  /**
   * Specifies whether the windowed mode window will be initially visible. This
   * hint is ignored for full screen windows.
   */
  bool visible = true;

  /**
   * Specifies whether the full screen window will automatically iconify and
   * restore the previous video mode on input focus loss. This hint is ignored
   * for windowed mode windows
   */
  bool auto_iconify = true;

  /**
   * @brief Specifies vsync mode.
   */
  bool enable_vsync = true;

  /**
   * @brief Specifies the desired refresh rate for full screen windows.
   * If set to WindowSettings::DontCare, the highest available refresh rate will
   * be used. This hint is ignored for windowed mode windows.
   */
  int32_t refresh_rate = WindowProps::DontCare;

  /**
   * @brief Specifies the default cursor mode of the window
   */
  Cursor::CursorMode cursor_mode = Cursor::CursorMode::NORMAL;

  /**
   * @brief Specifies the default cursor shape of the window
   */
  Cursor::CursorShape cursor_shape = Cursor::CursorShape::ARROW;

  /**
   * Defines the number of samples to use (For anti-aliasing)
   */
  uint32_t samples = 4;

  /**
   * Specify the client API major version that the created context must be
   * compatible with. The exact behavior of these hints depend on the requested
   * client API
   */
  uint8_t context_major_version = 4;

  /**
   * Specify the client API minor version that the created context must be
   * compatible with. The exact behavior of these hints depend on the requested
   * client API
   */
  uint8_t context_minor_version = 6;

  /**
   * specifies whether to create a debug OpenGL context, which may have
   * additional error and performance issue reporting functionality. If OpenGL
   * ES is requested, this hint is ignored
   */
  bool debug_profile = false;

  WindowProps(const std::string& t = "Xuzy's App", uint16_t w = 1280,
              uint16_t h = 720)
      : title(t), width(w), height(h) {}
};

}  // namespace xuzy::Window
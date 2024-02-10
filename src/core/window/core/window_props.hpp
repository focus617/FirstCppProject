
#pragma once

#include <string>
#include "window/core/cursor/cursor_mode.h"
#include "window/core/cursor/cursor_shape.h"

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

  WindowProps(const std::string& t = "Xuzy's App", uint16_t w = 1280,
              uint16_t h = 720)
      : title(t), width(w), height(h) {}
};

}  // namespace xuzy::Window
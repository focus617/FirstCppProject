#pragma once

#include "window/core/AGraphics_context.hpp"
#include "window/core/window_props.hpp"

namespace xuzy::Window {
/**
 * @brief Represents the OpenGL context
 */
class OpenGLContext : public AGraphicsContext {
 public:
  /**
   * @brief Creates the OpenGL context
   * @param p_props Settings
   */
  OpenGLContext(const WindowProps& p_props);

  /**
   * @brief Initialize the context
   */
  virtual void init() override;

  /**
   * @brief Dump backend driver vendor and version
   */
  virtual void vendor_info() override;

 public:
  uint m_major_version;
  uint m_minor_version;
  bool m_debug_profile;
};

}  // namespace xuzy::Window
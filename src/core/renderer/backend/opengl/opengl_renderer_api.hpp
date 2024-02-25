#pragma once

#include "renderer/core/ARenderer_api.hpp"

namespace xuzy::Renderer {

class OpenGLRendererAPI : public ARendererAPI {
 public:
  /**
   * @brief Initialize .
   */
  virtual void init() override;

  /**
   * @brief Set the viewport parameters.
   * @param x
   * @param y
   * @param width
   * @param height
   */
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) override;

  /**
   * @brief Define a clear color.
   * This color will be use for the next Renderer::Clear() call
   * @param p_red
   * @param p_green
   * @param p_blue
   * @param p_alpha
   */
  virtual void set_clear_color(float p_red, float p_green, float p_blue,
                               float p_alpha = 1.0f) override;

  /**
   * @brief Clear the screen using the previously defined clear color
   * (With Renderer::set_clear_color()) or by using the OpenGL default one.
   */
  virtual void clear() override;
};

}  // namespace xuzy::Renderer
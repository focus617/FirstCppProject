#pragma once

namespace xuzy::Renderer {

class ARendererAPI {
 public:
  enum class API { None = 0, OpenGL = 1 };

 public:
  virtual ~ARendererAPI() = default;

  /**
   * @brief Initialize .
   */
  virtual void init() = 0;

  /**
   * @brief Set the viewport parameters.
   * @param x
   * @param y
   * @param width
   * @param height
   */
  virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                            uint32_t height) = 0;

  /**
   * @brief Define a clear color.
   * This color will be use for the next Renderer::Clear() call
   * @param p_red
   * @param p_green
   * @param p_blue
   * @param p_alpha
   */
  virtual void set_clear_color(float p_red, float p_green, float p_blue,
                               float p_alpha = 1.0f) = 0;

  /**
   * @brief Clear the screen using the previously defined clear color
   * (With Renderer::set_clear_color()) or by using the OpenGL default one.
   */
  virtual void clear() = 0;

  static API get_api() { return s_api; }

  static Scope<ARendererAPI> Create();

 private:
  static API s_api;
};

}  // namespace xuzy::Renderer
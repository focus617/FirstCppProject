#pragma once

#include "renderer/renderer.h"
#include "tools/maths/FVector2.hpp"
#include "tools/maths/FVector3.hpp"
#include "tools/maths/FVector4.hpp"

namespace xuzy::Renderer {

class XUZY_API Renderer2D {
 public:
  static void init();
  static void shutdown();

  static void on_window_resize(uint32_t p_width, uint32_t p_height);

  static void begin_scene(Camera::OrthographicCamera& p_camera);
  static void end_scene();
  static void flush();

  // Primitives

  /**
   * @brief Draw quad with transform and color
   * @param p_position translate
   * @param p_size     scale
   * @param p_color    color
   */
  static void draw_quad(const Maths::FVector2& p_position,
                        const Maths::FVector2& p_size,
                        const Maths::FVector4& p_color);

  static void draw_quad(const Maths::FVector3& p_position,
                        const Maths::FVector2& p_size,
                        const Maths::FVector4& p_color);

  /**
   * @brief Draw quad with transform, texture and color
   * @param p_position translate
   * @param p_size     scale
   * @param p_texture  texture
   * @param p_tiling_factor 
   * @param p_tint_color color
   */
  static void draw_quad(
      const Maths::FVector2& p_position, const Maths::FVector2& p_size,
      const Ref<ATexture2D>& p_texture, float p_tiling_factor = 1.0f,
      const Maths::FVector4& p_tint_color = Maths::FVector4::One);

  static void draw_quad(
      const Maths::FVector3& p_position, const Maths::FVector2& p_size,
      const Ref<ATexture2D>& p_texture, float p_tiling_factor = 1.0f,
      const Maths::FVector4& p_tint_color = Maths::FVector4::One);

  /**
   * @brief Draw quad with rotation and color
   * @param p_position translate
   * @param p_size     scale
   * @param p_rotation in degree
   * @param p_color    color
   */
  static void draw_rotated_quad(const Maths::FVector2& p_position,
                                const Maths::FVector2& p_size, float p_rotation,
                                const Maths::FVector4& p_color);

  static void draw_rotated_quad(const Maths::FVector3& p_position,
                                const Maths::FVector2& p_size, float p_rotation,
                                const Maths::FVector4& p_color);

  /**
   * @brief Draw quad with transform, texture and color
   * @param p_position translate
   * @param p_size     scale
   * @param p_rotation in degree
   * @param p_texture  texture
   * @param p_tiling_factor 
   * @param p_tint_color color
   */
  static void draw_rotated_quad(
      const Maths::FVector2& p_position, const Maths::FVector2& p_size,
      float p_rotation, const Ref<ATexture2D>& p_texture,
      float p_tiling_factor = 1.0f,
      const Maths::FVector4& p_tint_color = Maths::FVector4::One);

  static void draw_rotated_quad(
      const Maths::FVector3& p_position, const Maths::FVector2& p_size,
      float p_rotation, const Ref<ATexture2D>& p_texture,
      float p_tiling_factor = 1.0f,
      const Maths::FVector4& p_tint_color = Maths::FVector4::One);
};

}  // namespace xuzy::Renderer
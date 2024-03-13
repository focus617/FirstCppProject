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

  // Primitives
  static void draw_quad(const Maths::FVector2& p_position,
                        const Maths::FVector2& p_size,
                        const Maths::FVector4& p_color);

  static void draw_quad(const Maths::FVector3& p_position,
                        const Maths::FVector2& p_size,
                        const Maths::FVector4& p_color);

  static void draw_quad(const Maths::FVector2& p_position,
                        const Maths::FVector2& p_size,
                        const Ref<ATexture2D>& p_texture);

  static void draw_quad(const Maths::FVector3& p_position,
                        const Maths::FVector2& p_size,
                        const Ref<ATexture2D>& p_texture);
};

}  // namespace xuzy::Renderer
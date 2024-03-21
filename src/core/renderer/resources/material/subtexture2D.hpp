#pragma once

#include "renderer/resources/material/ATexture2D.hpp"
#include "tools/maths/FVector2.hpp"

namespace xuzy::Renderer {

/**
 * @brief 2D texture class
 */
class XUZY_API SubTexture2D {
 public:
  SubTexture2D(const Ref<ATexture2D>& p_texture, const Maths::FVector2& p_min,
               const Maths::FVector2& p_max);

  const Ref<ATexture2D> get_texture() const { return m_texture_; }
  const Maths::FVector2* get_tex_coords() const { return m_texture_coords_; }

  static Ref<SubTexture2D> CreateFromCoords(
      const Ref<ATexture2D>& p_texture, const Maths::FVector2& p_coords,
      const Maths::FVector2& p_cell_size,
      const Maths::FVector2& p_sprite_size = {1, 1});

 private:
  Ref<ATexture2D> m_texture_;

  Maths::FVector2 m_texture_coords_[4];
};

}  // namespace xuzy::Renderer
#include "pch.h"

#include "renderer/resources/material/subtexture2D.hpp"

#include "renderer/renderer/renderer.hpp"

namespace xuzy::Renderer {

SubTexture2D::SubTexture2D(const Ref<ATexture2D>& p_texture,
                           const Maths::FVector2& p_min,
                           const Maths::FVector2& p_max)
    : m_texture_(p_texture) {
  m_texture_coords_[0] = {p_min.x, p_min.y};  // Bottom-Left
  m_texture_coords_[1] = {p_max.x, p_min.y};  // Bottom-Right
  m_texture_coords_[2] = {p_max.x, p_max.y};  // Top-Right
  m_texture_coords_[3] = {p_min.x, p_max.y};  // Top-Left
}

Ref<SubTexture2D> SubTexture2D::CreateFromCoords(
    const Ref<ATexture2D>& p_texture, const Maths::FVector2& p_coords,
    const Maths::FVector2& p_cell_size, const Maths::FVector2& p_sprite_size) {
  Maths::FVector2 min = {
      (p_coords.x * p_cell_size.x) / p_texture->get_width(),
      (p_coords.y * p_cell_size.y) / p_texture->get_height()};

  Maths::FVector2 max = {
      ((p_coords.x + p_sprite_size.x) * p_cell_size.x) / p_texture->get_width(),
      ((p_coords.y + p_sprite_size.y) * p_cell_size.y) /
          p_texture->get_height()};

  return CreateRef<SubTexture2D>(p_texture, min, max);
}

}  // namespace xuzy::Renderer
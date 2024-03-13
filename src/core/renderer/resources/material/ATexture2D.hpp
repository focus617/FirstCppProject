#pragma once

#include "renderer/resources/material/ATexture.hpp"

namespace xuzy::Renderer {

/**
 * @brief Generic 2D texture class
 */
class XUZY_API ATexture2D : public ATexture {
 public:
  static Ref<ATexture2D> Create(uint32_t p_width, uint32_t p_height);
  static Ref<ATexture2D> Create(const std::string& p_path);
};

}  // namespace xuzy::Renderer
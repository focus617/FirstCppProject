#pragma once

#include <glad/glad.h>

#include "renderer/resources/material/ATexture.hpp"

namespace xuzy::Renderer {

/**
 * @brief OpenGL 2D texture wrapper
 */
class OpenGLTexture2D : public ATexture2D {
 public:
  OpenGLTexture2D(uint32_t p_width, uint32_t p_height);
  OpenGLTexture2D(const std::string& p_path);
  virtual ~OpenGLTexture2D();

  virtual uint32_t get_width() const override { return m_width_; }
  virtual uint32_t get_height() const override { return m_height_; }
  virtual uint32_t get_renderer_id() const override { return m_renderer_id_; }

  virtual void set_data(void* p_data, uint32_t p_size) override;

  /**
   * @brief Bind the texture to the given slot
   * @param p_slot
   */
  virtual void bind(uint32_t p_slot = 0) const override;

  /**
   * @brief Unbind the texture
   */
  virtual void unbind() const override;

  virtual bool is_loaded() const override { return m_is_loaded_; }

  virtual bool operator==(const ATexture& other) const override {
    return m_renderer_id_ == ((OpenGLTexture2D&)other).m_renderer_id_;
  }

 private:
  std::string m_path_;
  bool m_is_loaded_ = false;
  uint32_t m_width_, m_height_;
  uint32_t m_renderer_id_;
  GLenum m_data_format_;
  GLenum m_internal_format_;  // describe how opengl storge the texture data
};

}  // namespace xuzy::Renderer
#include "pch.h"

#include "renderer/backend/opengl/opengl_texture2D.hpp"

#include <stb_image.h>

namespace xuzy::Renderer {

OpenGLTexture2D::OpenGLTexture2D(uint32_t p_width, uint32_t p_height)
    : m_width_(p_width), m_height_(p_height) {
  m_internal_format_ = GL_RGBA8;
  m_data_format_ = GL_RGBA;

  glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id_);
  glTextureStorage2D(m_renderer_id_, 1, m_internal_format_, m_width_,
                     m_height_);

  glTextureParameteri(m_renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(m_renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTextureParameteri(m_renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(m_renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& p_path) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = nullptr;

  data = stbi_load(p_path.c_str(), &width, &height, &channels, 0);
  if (data) {
    m_is_loaded_ = true;

    m_width_ = width;
    m_height_ = height;

    GLenum internal_format = 0, data_format = 0;
    if (channels == 4) {
      internal_format = GL_RGBA8;
      data_format = GL_RGBA;
    } else if (channels == 3) {
      internal_format = GL_RGB8;
      data_format = GL_RGB;
    }

    m_internal_format_ = internal_format;
    m_data_format_ = data_format;

    XUZY_CHECK_(internal_format & data_format) << "Format not supported!";

    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id_);
    glTextureStorage2D(m_renderer_id_, 1, internal_format, m_width_, m_height_);

    glTextureParameteri(m_renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_renderer_id_, 0, 0, 0, m_width_, m_height_,
                        m_data_format_, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &m_renderer_id_); }

void OpenGLTexture2D::set_data(void* p_data, uint32_t p_size) {
  uint32_t bpp = m_data_format_ == GL_RGBA ? 4 : 3;

  XUZY_CHECK_(p_size == m_width_ * m_height_ * bpp)
      << "Data must be entire texture!";

  glTextureSubImage2D(m_renderer_id_, 0, 0, 0, m_width_, m_height_,
                      m_data_format_, GL_UNSIGNED_BYTE, p_data);
}

void OpenGLTexture2D::bind(uint32_t p_slot) const {
  glBindTextureUnit(p_slot, m_renderer_id_);
}

void OpenGLTexture2D::unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

}  // namespace xuzy::Renderer
#include "renderer/backend/opengl/opengl_frame_buffer.hpp"

#include <glad/glad.h>

namespace xuzy::Renderer::Buffer {

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& p_spec)
    : m_specification_(p_spec) {
  create_framebuffer();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() { delete_framebuffer(); }

void OpenGLFrameBuffer::create_framebuffer() {
  glCreateFramebuffers(1, &m_renderer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id_);

  glCreateTextures(GL_TEXTURE_2D, 1, &m_color_attachment_);
  glBindTexture(GL_TEXTURE_2D, m_color_attachment_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification_.width,
               m_specification_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_color_attachment_, 0);

  glCreateTextures(GL_TEXTURE_2D, 1, &m_depth_attachment_);
  glBindTexture(GL_TEXTURE_2D, m_depth_attachment_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_specification_.width,
                 m_specification_.height);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, m_depth_attachment_, 0);

  XUZY_CHECK_(glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
              GL_FRAMEBUFFER_COMPLETE)
      << "Framebuffer is incomplete!";

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::delete_framebuffer() {
  glDeleteFramebuffers(1, &m_renderer_id_);
  glDeleteTextures(1, &m_color_attachment_);
  glDeleteTextures(1, &m_depth_attachment_);
}

void OpenGLFrameBuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id_);
  glViewport(0, 0, m_specification_.width, m_specification_.height);
}

void OpenGLFrameBuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFrameBuffer::resize(uint32_t p_width, uint32_t p_height) {
  LOG(INFO) << "New Size: " << p_width << ", " << p_height;

  m_specification_.width = p_width;
  m_specification_.height = p_height;

  if (m_renderer_id_) {
    delete_framebuffer();
    m_renderer_id_ = 0;
  }

  create_framebuffer();
}

}  // namespace xuzy::Renderer::Buffer
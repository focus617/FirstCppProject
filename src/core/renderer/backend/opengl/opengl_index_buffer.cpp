#include <glad/glad.h>

#include "renderer/backend/opengl/opengl_index_buffer.hpp"

namespace xuzy::Renderer::Buffer {

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* p_indices, uint32_t p_count)
    : m_count_(p_count) {
  glCreateBuffers(1, &m_renderer_id_);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless
  // of VAO state.
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, p_count * sizeof(uint32_t), p_indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &m_renderer_id_); }

void OpenGLIndexBuffer::bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id_);
}

void OpenGLIndexBuffer::unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}  // namespace xuzy::Renderer::Buffer
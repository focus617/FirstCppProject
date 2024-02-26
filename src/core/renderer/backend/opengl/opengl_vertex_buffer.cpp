#include "renderer/backend/opengl/opengl_vertex_buffer.hpp"

#include <glad/glad.h>

namespace xuzy::Renderer::Buffer {

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t p_size) {
  glCreateBuffers(1, &m_renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, p_size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(void* p_vertices, uint32_t p_size) {
  glCreateBuffers(1, &m_renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, p_size, p_vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  glDeleteBuffers(1, &m_renderer_id_);
}

void OpenGLVertexBuffer::bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_);
}

void OpenGLVertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::set_data(const void* p_data, uint32_t p_size) {
  glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, p_size, p_data);
}

const BufferLayout& OpenGLVertexBuffer::get_layout() const { return m_layout_; }

void OpenGLVertexBuffer::set_layout(const BufferLayout& p_layout) {
  m_layout_ = p_layout;
}

}  // namespace xuzy::Renderer::Buffer
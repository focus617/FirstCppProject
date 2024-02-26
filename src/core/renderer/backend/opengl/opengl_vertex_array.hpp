#pragma once

#include <vector>

#include "renderer/buffers/AVertex_array.hpp"

namespace xuzy::Renderer::Buffer {

/**
 * @brief Wraps OpenGL VAO
 */
class OpenGLVertexArray : public AVertexArray {
 public:
  /**
   * @brief OpenGL VertexArray Constructor
   */
  OpenGLVertexArray();

  /**
   * @brief Destructor
   */
  virtual ~OpenGLVertexArray();

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const;

  /**
   * @brief Assign VertexBuffer to this VertexArray
   */
  virtual void add_vertex_buffer(const Ref<AVertexBuffer>& p_vertex_buffer);

  /**
   * @brief Assign IndexBuffer to this VertexArray
   */
  virtual void set_index_buffer(const Ref<AIndexBuffer>& p_index_buffer);

  /**
   * @brief Returns VertexBuffer of this VertexArray
   */
  virtual const std::vector<Ref<AVertexBuffer>>& get_vertex_buffers() const {
    return m_vertex_buffers_;
  }

  /**
   * @brief Returns IndexBuffer of this VertexArray
   */
  virtual const Ref<AIndexBuffer>& get_index_buffer() const {
    return m_index_buffer_;
  }

 private:
  uint32_t m_renderer_id_;
  uint32_t m_vertex_buffer_index_ = 0;
  std::vector<Ref<AVertexBuffer>> m_vertex_buffers_;
  Ref<AIndexBuffer> m_index_buffer_;
};

}  // namespace xuzy::Renderer::Buffer
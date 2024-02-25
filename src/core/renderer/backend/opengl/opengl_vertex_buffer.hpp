#pragma once

#include <vector>

#include "renderer/buffers/AVertex_buffer.hpp"

namespace xuzy::Renderer::Buffer {

/**
 * @brief Wraps OpenGL VBO
 */
class OpenGLVertexBuffer : public AVertexBuffer {
 public:
  /**
   * @brief Create the VBO using a pointer to the first element and a size
   * (number of elements)
   * @param p_size  Total size of vertices in byte
   */
  OpenGLVertexBuffer(uint32_t p_size);

  /**
   * @brief Create the VBO using a pointer to the first element and a size
   * (number of elements)
   * @param p_vertices Pointer to first byte of vertices
   * @param p_size     Total size of vertices in byte
   */
  OpenGLVertexBuffer(void* p_vertices, uint32_t p_size);

  /**
   * @brief Destructor
   */
  virtual ~OpenGLVertexBuffer();

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const override;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const override;

  /**
   * @brief Fill the VBO using a pointer to the first element and a size
   * (number of elements)
   * @param p_data
   * @param p_size
   */
  virtual void set_data(const void* p_data, uint32_t p_size) override;

  /**
   * @brief Returnd the Layout of the VBO
   */
  // virtual const BufferLayout& get_layout() const override;

  /**
   * @brief Setup the Layout of the VBO
   */
  // virtual void set_layout(const BufferLayout& layout) override;

  /**
   * @brief Returnd the ID of the VBO
   */
  uint32_t get_id() { return m_renderer_id_; }

 private:
  uint32_t m_renderer_id_;
  // BufferLayout m_layout_;
};

}  // namespace xuzy::Renderer::Buffer
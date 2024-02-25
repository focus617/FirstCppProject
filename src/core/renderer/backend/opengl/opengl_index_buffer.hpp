#pragma once

#include <vector>

#include "renderer/buffers/AIndex_buffer.hpp"

namespace xuzy::Renderer::Buffer {

/**
 * @brief Wraps OpenGL EBO
 */
class OpenGLIndexBuffer : public AIndexBuffer {
 public:
  /**
   * @brief Create the EBO using a pointer to the first element and a size
   * (number of elements)
   * @param p_indices
   * @param p_count
   */
  OpenGLIndexBuffer(uint32_t* p_indices, uint32_t p_count);

  /**
   * @brief Destructor
   */
  virtual ~OpenGLIndexBuffer();

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const override;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const override;

  /**
   * @brief Returns the indices count in buffer
   */
  virtual uint32_t get_count() const override { return m_count_; }

 private:
  uint32_t m_renderer_id_;
  uint32_t m_count_;
};

}  // namespace xuzy::Renderer::Buffer
#pragma once

#include <vector>

namespace xuzy::Renderer::Buffer {

/**
 * @brief Generic EBO
 * Limitation: currently only supports 32-bit index buffers
 */
class XUZY_API AIndexBuffer {
 public:
  /**
   * @brief Concrete indexbuffer creater
   * @param p_indices
   * @param p_count
   */
  static Ref<AIndexBuffer> Create(uint32_t* p_indices, uint32_t p_count);

  /**
   * @brief Destructor
   */
  virtual ~AIndexBuffer() = default;

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const = 0;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const = 0;

  /**
   * @brief Returns the indices count in buffer
   */
  virtual uint32_t get_count() const = 0;
};

}  // namespace xuzy::Renderer::Buffer
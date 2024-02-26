#pragma once

#include <vector>

#include "renderer/buffers/buffer_layout.hpp"

namespace xuzy::Renderer::Buffer {

/**
 * @brief Generic VBO
 */
class XUZY_API AVertexBuffer {
 public:
  /**
   * @brief Concrete VertexBuffer creater
   * @param p_size Total size of vertices in byte
   */
  static Ref<AVertexBuffer> Create(uint32_t p_size);

  /**
   * @brief Concrete vertexbuffer creater
   * @param p_vertices Pointer to first byte of vertices
   * @param p_size Total size of vertices in byte
   */
  static Ref<AVertexBuffer> Create(void* p_vertices, uint32_t p_size);

  /**
   * @brief Destructor
   */
  virtual ~AVertexBuffer() = default;

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const = 0;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const = 0;

  /**
   * @brief Fill the VBO using a pointer to the first element and a size
   * (number of elements)
   * @param p_data
   * @param p_size
   */
  virtual void set_data(const void* p_data, uint32_t p_size) = 0;

  /**
   * @brief Returnd the Layout of the VBO
   */
  virtual const BufferLayout& get_layout() const = 0;

  /**
   * @brief Setup the Layout of the VBO
   */
  virtual void set_layout(const BufferLayout& layout) = 0;
};

}  // namespace xuzy::Renderer::Buffer
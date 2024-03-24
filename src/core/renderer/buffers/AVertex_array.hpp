#pragma once

#include "renderer/buffers/AIndex_buffer.hpp"
#include "renderer/buffers/AVertex_buffer.hpp"

namespace xuzy::Renderer::Buffer {

class XUZY_API AVertexArray {
 public:
  /**
   * @brief Concrete VertexArray creater
   */
  static Ref<AVertexArray> Create();

  /**
   * @brief Destructor
   */
  virtual ~AVertexArray() = default;

  /**
   * @brief Bind the buffer
   */
  virtual void bind() const = 0;

  /**
   * @brief Unbind the buffer
   */
  virtual void unbind() const = 0;

  /**
   * @brief Assign VertexBuffer to this VertexArray
   */
  virtual void add_vertex_buffer(const Ref<AVertexBuffer>& p_vertex_buffer) = 0;

  /**
   * @brief Assign IndexBuffer to this VertexArray
   */
  virtual void set_index_buffer(const Ref<AIndexBuffer>& p_index_buffer) = 0;

  /**
   * @brief Returns VertexBuffers of this VertexArray
   */
  virtual const std::vector<Ref<AVertexBuffer>>& get_vertex_buffers() const = 0;

  /**
   * @brief Returns IndexBuffer of this VertexArray
   */
  virtual const Ref<AIndexBuffer>& get_index_buffer() const = 0;
};

}  // namespace xuzy::Renderer::Buffer
#pragma once

#include "renderer/buffers/AFrame_buffer.hpp"

namespace xuzy::Renderer::Buffer {

/**
 * @brief Wraps OpenGL FBO
 */
class OpenGLFrameBuffer : public AFrameBuffer {
 public:
  /**
   * @brief Constructor
   */
  OpenGLFrameBuffer(const FrameBufferSpecification& p_spec);

  /**
   * @brief Destructor
   */
  virtual ~OpenGLFrameBuffer();

  /**
   * @brief Bind the framebuffer
   */
  virtual void bind() override;

  /**
   * @brief Unbind the framebuffer
   */
  virtual void unbind() override;

  /**
   * @brief Resize the framebuffer
   */
  virtual void resize(uint32_t p_width, uint32_t p_height) override;

  /**
   * @brief Returns reference of color attachment
   */
  virtual uint32_t get_color_attachment_renderer_id() const override {
    return m_color_attachment_;
  }

  /**
   * @brief Returns the specification of framebuffer
   */
  virtual const FrameBufferSpecification& get_specification() const override {
    return m_specification_;
  }

 private:
  void create_framebuffer();
  void delete_framebuffer();

 private:
  uint32_t m_renderer_id_ = 0;
  uint32_t m_color_attachment_ = 0, m_depth_attachment_ = 0;
  FrameBufferSpecification m_specification_;
};

}  // namespace xuzy::Renderer::Buffer

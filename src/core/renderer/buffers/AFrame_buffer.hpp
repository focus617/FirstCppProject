#pragma once

namespace xuzy::Renderer::Buffer {

struct XUZY_API FrameBufferSpecification {
  uint32_t width, height;
  // FramebufferFormat Format =
  uint32_t samples = 1;

  bool swap_chain_target = false;  // whether renderer to screen
};

/**
 * @brief Generic FBO
 */
class XUZY_API AFrameBuffer {
 public:
  /**
   * @brief Concrete framebuffer creater
   * @param p_spec
   */
  static Ref<AFrameBuffer> Create(const FrameBufferSpecification& p_spec);

   /**
   * @brief Destructor
   */
  virtual ~AFrameBuffer() = default;

  /**
   * @brief Bind the framebuffer
   */
  virtual void bind() = 0;

  /**
   * @brief Unbind the framebuffer
   */
  virtual void unbind() = 0;

  /**
   * @brief Resize the framebuffer
   */
  virtual void resize(uint32_t p_width, uint32_t p_height) = 0;

  /**
   * @brief Returns reference of color attachment
   */
  virtual uint32_t get_color_attachment_renderer_id() const = 0;

  /**
   * @brief Returns the specification of framebuffer
   */
  virtual const FrameBufferSpecification& get_specification() const = 0;

};

}  // namespace xuzy::Renderer::Buffer
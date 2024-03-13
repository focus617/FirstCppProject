#pragma once

namespace xuzy::Renderer {

/**
 * @brief Generic texture class
 */
class XUZY_API ATexture {
 public:
  /**
   * @brief Default destructor
   */
  virtual ~ATexture() = default;

  /**
   * @brief Return width of texture
   */
  virtual uint32_t get_width() const = 0;

  /**
   * @brief Return height of texture
   */
  virtual uint32_t get_height() const = 0;

  /**
   * @brief Return id of texture
   */
  virtual uint32_t get_renderer_id() const = 0;

  /**
   * @brief Setup the data of entire texture
   * @param p_data
   * @param p_size
   * Note: p_size == m_width_ * m_height_ * bytes_per_pixel
   */
  virtual void set_data(void* p_data, uint32_t p_size) = 0;

  /**
   * @brief Bind the texture to the given slot
   * @param p_slot
   */
  virtual void bind(uint32_t p_slot = 0) const = 0;

  /**
   * @brief Unbind the texture
   */
  virtual void unbind() const = 0;

  /**
   * @brief Return true if texture has been loaded
   */
  virtual bool is_loaded() const = 0;

  virtual bool operator==(const ATexture& other) const = 0;
};

}  // namespace xuzy::Renderer
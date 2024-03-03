#pragma once

namespace xuzy::Renderer {

/**
 * @brief Generic texture class
 */
class XUZY_API ATexture {
 public:
  virtual ~ATexture() = default;

  virtual uint32_t get_width() const = 0;
  virtual uint32_t get_height() const = 0;
  virtual uint32_t get_renderer_id() const = 0;

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

  virtual bool is_loaded() const = 0;

  virtual bool operator==(const ATexture& other) const = 0;
};

class XUZY_API ATexture2D : public ATexture {
 public:
  static Ref<ATexture2D> Create(uint32_t p_width, uint32_t p_height);
  static Ref<ATexture2D> Create(const std::string& p_path);
};

}  // namespace xuzy::Renderer
#pragma once

namespace xuzy::Renderer::Buffer {

enum class ShaderDataType {
  None = 0,
  Float,
  FVec2,
  FVec3,
  FVec4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::FVec2:
      return 4 * 2;
    case ShaderDataType::FVec3:
      return 4 * 3;
    case ShaderDataType::FVec4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    default: {
      LOG(ERROR) << "Unknown ShaderDataType!";
      return 0;
    }
  }
  LOG(ERROR) << "Unknown ShaderDataType!";
  return 0;
}

struct BufferElement {
  std::string name;
  ShaderDataType type;
  uint32_t size;
  size_t offset;
  bool normalized;

  BufferElement() = default;

  BufferElement(ShaderDataType p_type, const std::string& p_name,
                bool p_normalized = false)
      : name(p_name),
        type(p_type),
        size(ShaderDataTypeSize(p_type)),
        offset(0),
        normalized(p_normalized) {}

  uint32_t get_component_count() const {
    switch (type) {
      case ShaderDataType::Float:
        return 1;
      case ShaderDataType::FVec2:
        return 2;
      case ShaderDataType::FVec3:
        return 3;
      case ShaderDataType::FVec4:
        return 4;
      case ShaderDataType::Mat3:
        return 3;  // 3* FVec3
      case ShaderDataType::Mat4:
        return 4;  // 4* FVec4
      case ShaderDataType::Int:
        return 1;
      case ShaderDataType::Int2:
        return 2;
      case ShaderDataType::Int3:
        return 3;
      case ShaderDataType::Int4:
        return 4;
      case ShaderDataType::Bool:
        return 1;
      default: {
        LOG(ERROR) << "Unknown ShaderDataType!";
        return 0;
      }
    }
  }
};

class XUZY_API BufferLayout {
 public:
  BufferLayout() {}

  BufferLayout(std::initializer_list<BufferElement> elements)
      : m_elements_(elements) {
    calculate_offsets_and_stride();
  }

  uint32_t get_stride() const { return m_stride_; }
  const std::vector<BufferElement>& get_elements() const { return m_elements_; }

  std::vector<BufferElement>::iterator begin() { return m_elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return m_elements_.end(); }
  std::vector<BufferElement>::const_iterator begin() const {
    return m_elements_.begin();
  }
  std::vector<BufferElement>::const_iterator end() const {
    return m_elements_.end();
  }

 private:
  void calculate_offsets_and_stride() {
    size_t offset = 0;
    m_stride_ = 0;
    for (auto& element : m_elements_) {
      element.offset = offset;
      offset += element.size;
      m_stride_ += element.size;
    }
  }

 private:
  std::vector<BufferElement> m_elements_;
  uint32_t m_stride_ = 0;
};

}  // namespace xuzy::Renderer::Buffer
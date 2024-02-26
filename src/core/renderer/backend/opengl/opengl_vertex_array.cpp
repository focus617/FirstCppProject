#include "renderer/backend/opengl/opengl_vertex_array.hpp"

#include <glad/glad.h>

#include "renderer/buffers/buffer_layout.hpp"

namespace xuzy::Renderer::Buffer {

static GLenum shader_datatype_to_OpenGL_basetype(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
      return GL_FLOAT;
    case ShaderDataType::FVec2:
      return GL_FLOAT;
    case ShaderDataType::FVec3:
      return GL_FLOAT;
    case ShaderDataType::FVec4:
      return GL_FLOAT;
    case ShaderDataType::Mat3:
      return GL_FLOAT;
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
      return GL_INT;
    case ShaderDataType::Int2:
      return GL_INT;
    case ShaderDataType::Int3:
      return GL_INT;
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    default:
      LOG(ERROR) << "Unknown ShaderDataType!";
  }
  LOG(ERROR) << "Unknown ShaderDataType!";
  return 0;
}

OpenGLVertexArray::OpenGLVertexArray() {
  glCreateVertexArrays(1, &m_renderer_id_);
}

OpenGLVertexArray::~OpenGLVertexArray() {
  glDeleteVertexArrays(1, &m_renderer_id_);
}

void OpenGLVertexArray::bind() const { glBindVertexArray(m_renderer_id_); }

void OpenGLVertexArray::unbind() const { glBindVertexArray(0); }

void OpenGLVertexArray::add_vertex_buffer(
    const Ref<AVertexBuffer>& p_vertex_buffer) {
  XUZY_CHECK_(p_vertex_buffer->get_layout().get_elements().size())
      << "Vertex Buffer has no layout!";

  glBindVertexArray(m_renderer_id_);
  p_vertex_buffer->bind();

  const auto& layout = p_vertex_buffer->get_layout();
  for (const auto& element : layout) {
    switch (element.type) {
      case ShaderDataType::Float:
      case ShaderDataType::FVec2:
      case ShaderDataType::FVec3:
      case ShaderDataType::FVec4: {
        glEnableVertexAttribArray(m_vertex_buffer_index_);
        glVertexAttribPointer(m_vertex_buffer_index_,
                              element.get_component_count(),
                              shader_datatype_to_OpenGL_basetype(element.type),
                              element.normalized ? GL_TRUE : GL_FALSE,
                              layout.get_stride(), (const void*)element.offset);
        m_vertex_buffer_index_++;
        break;
      }
      case ShaderDataType::Int:
      case ShaderDataType::Int2:
      case ShaderDataType::Int3:
      case ShaderDataType::Int4:
      case ShaderDataType::Bool: {
        glEnableVertexAttribArray(m_vertex_buffer_index_);
        glVertexAttribIPointer(
            m_vertex_buffer_index_, element.get_component_count(),
            shader_datatype_to_OpenGL_basetype(element.type),
            layout.get_stride(), (const void*)element.offset);
        m_vertex_buffer_index_++;
        break;
      }
      case ShaderDataType::Mat3:
      case ShaderDataType::Mat4: {
        uint8_t count = element.get_component_count();
        for (uint8_t i = 0; i < count; i++) {
          glEnableVertexAttribArray(m_vertex_buffer_index_);
          glVertexAttribPointer(
              m_vertex_buffer_index_, count,
              shader_datatype_to_OpenGL_basetype(element.type),
              element.normalized ? GL_TRUE : GL_FALSE, layout.get_stride(),
              (const void*)(element.offset + sizeof(float) * count * i));
          glVertexAttribDivisor(m_vertex_buffer_index_, 1);
          m_vertex_buffer_index_++;
        }
        break;
      }
      default:
        LOG(ERROR) << "Unknown ShaderDataType!";
    }
  }

  m_vertex_buffers_.push_back(p_vertex_buffer);
}

void OpenGLVertexArray::set_index_buffer(
    const Ref<AIndexBuffer>& p_index_buffer) {
  glBindVertexArray(m_renderer_id_);
  p_index_buffer->bind();

  m_index_buffer_ = p_index_buffer;
}

}  // namespace xuzy::Renderer::Buffer
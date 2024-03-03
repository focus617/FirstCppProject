#include "pch.h"

#include "renderer/buffers/AVertex_buffer.hpp"

#include "renderer/backend/opengl/opengl_vertex_buffer.hpp"
#include "renderer/renderer/renderer.hpp"

namespace xuzy::Renderer::Buffer {

Ref<AVertexBuffer> AVertexBuffer::Create(uint32_t p_size) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLVertexBuffer>(p_size);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

Ref<AVertexBuffer> AVertexBuffer::Create(void* p_vertices, uint32_t p_size) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLVertexBuffer>(p_vertices, p_size);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer::Buffer
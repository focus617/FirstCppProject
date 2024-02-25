#include "pch.h"

#include "renderer/buffers/AVertex_buffer.hpp"

#include "renderer/backend/opengl/opengl_index_buffer.hpp"
#include "renderer/core/renderer.hpp"

namespace xuzy::Renderer::Buffer {

Ref<AIndexBuffer> AIndexBuffer::Create(uint32_t* p_indices, uint32_t p_count) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLIndexBuffer>(p_indices, p_count);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer::Buffer
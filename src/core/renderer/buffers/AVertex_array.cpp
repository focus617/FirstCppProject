#include "pch.h"

#include "renderer/buffers/AVertex_array.hpp"
#include "renderer/backend/opengl/opengl_vertex_array.hpp"
#include "renderer/core/renderer.hpp"

namespace xuzy::Renderer::Buffer {

Ref<AVertexArray> AVertexArray::Create() {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLVertexArray>();
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer::Buffer
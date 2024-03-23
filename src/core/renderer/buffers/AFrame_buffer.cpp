#include "pch.h"

#include "renderer/buffers/AFrame_buffer.hpp"

#include "renderer/backend/opengl/opengl_frame_buffer.hpp"
#include "renderer/renderer/renderer.hpp"

namespace xuzy::Renderer::Buffer {

Ref<AFrameBuffer> AFrameBuffer::Create(const FrameBufferSpecification& p_spec) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLFrameBuffer>(p_spec);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer::Buffer
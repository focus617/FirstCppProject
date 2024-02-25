#include "pch.h"

#include "renderer/core/ARenderer_api.hpp"

#include "renderer/backend/opengl/opengl_renderer_api.hpp"

namespace xuzy::Renderer {

ARendererAPI::API ARendererAPI::s_api = ARendererAPI::API::OpenGL;

Scope<ARendererAPI> ARendererAPI::Create() {
  switch (s_api) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateScope<OpenGLRendererAPI>();
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer
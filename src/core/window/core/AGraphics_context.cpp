#include "window/core/AGraphics_context.hpp"

#include "renderer/core/renderer.hpp"
#include "window/backend_glfw/opengl_context.hpp"

namespace xuzy::Window {

Scope<AGraphicsContext> AGraphicsContext::Create(const WindowProps& p_props) {
  switch (Renderer::Renderer::get_api()) {
    case Renderer::ARendererAPI::API::None: {
      LOG(ERROR) << "ARendererAPI::None is currently not supported!";
      return nullptr;
    }

    case Renderer::ARendererAPI::API::OpenGL: {
      return CreateScope<OpenGLContext>(p_props);
    }
  }

  LOG(ERROR) << "Unknown ARendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Window
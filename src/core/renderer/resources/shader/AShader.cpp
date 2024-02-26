#include "pch.h"

#include "renderer/resources/shader/AShader.hpp"

#include "renderer/backend/opengl/opengl_renderer_api.hpp"
#include "renderer/backend/opengl/opengl_shader.hpp"
#include "renderer/core/renderer.hpp"

namespace xuzy::Renderer {

Ref<AShader> AShader::Create(const std::string& p_filepath) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLShader>(p_filepath);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

Ref<AShader> AShader::Create(const std::string& p_name,
                           const std::string& p_vertex_src,
                           const std::string& p_fragment_src) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLShader>(p_name, p_vertex_src, p_fragment_src);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer
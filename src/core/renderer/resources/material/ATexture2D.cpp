#include "pch.h"

#include "renderer/resources/material/ATexture2D.hpp"

#include "renderer/backend/opengl/opengl_texture2D.hpp"
#include "renderer/renderer/renderer.hpp"

namespace xuzy::Renderer {

Ref<ATexture2D> ATexture2D::Create(uint32_t p_width, uint32_t p_height) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLTexture2D>(p_width, p_height);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

Ref<ATexture2D> ATexture2D::Create(const std::string& p_path) {
  switch (Renderer::get_api()) {
    case ARendererAPI::API::None: {
      LOG(ERROR) << "RendererAPI::None is currently not supported!";
      return nullptr;
    }

    case ARendererAPI::API::OpenGL: {
      return CreateRef<OpenGLTexture2D>(p_path);
    }
  }

  LOG(ERROR) << "Unknown RendererAPI!";
  return nullptr;
}

}  // namespace xuzy::Renderer
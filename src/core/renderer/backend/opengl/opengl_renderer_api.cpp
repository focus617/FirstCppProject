#include "pch.h"

#include "renderer/backend/opengl/opengl_renderer_api.hpp"

#include <glad/glad.h>

namespace xuzy::Renderer {

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id,
                           unsigned severity, int length, const char* message,
                           const void* userParam) {
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
      LOG(FATAL) << message;
      return;
    }
    case GL_DEBUG_SEVERITY_MEDIUM: {
      LOG(ERROR) << message;
      return;
    }
    case GL_DEBUG_SEVERITY_LOW: {
      LOG(WARNING) << message;
      return;
    }
    case GL_DEBUG_SEVERITY_NOTIFICATION: {
      LOG(INFO) << message;
      return;
    }
  }

  LOG(ERROR) << "Unknown severity level!";
}

void OpenGLRendererAPI::init() {
#ifdef _DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenGLMessageCallback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                        GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::set_clear_color(float p_red, float p_green,
                                        float p_blue, float p_alpha) {
  glClearColor(p_red, p_green, p_blue, p_alpha);
}

void OpenGLRendererAPI::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::draw_indexed(
    const Ref<Renderer::Buffer::AVertexArray>& p_vertex_array,
    uint32_t p_index_count) {
  uint32_t count = p_index_count
                       ? p_index_count
                       : p_vertex_array->get_index_buffer()->get_count();
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

}  // namespace xuzy::Renderer
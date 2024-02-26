#pragma once

#include "core.h"

#include "renderer/buffers/AVertex_array.hpp"
#include "renderer/resources/shader/AShader.hpp"
#include "window/core/layer.hpp"

namespace xuzy::UI {
/**
 * @brief A Layer used for experimental purpose
 */
class XUZY_API ExampleLayer : public xuzy::Window::ALayer {
 public:
  /**
   * @brief Constructor
   */
  ExampleLayer();

  /**
   * @brief Destructor
   */
  ~ExampleLayer();

  virtual void on_attach() override { LOG(INFO) << "ExampleLayer OnAttach"; }
  virtual void on_detach() override { LOG(INFO) << "ExampleLayer OnDetach"; }
  virtual void on_update() override;
  virtual void on_event(Ref<Events::Event> event, bool& handled) override;

  /**
   * @brief Render its components
   */
  virtual void on_draw() override;

 private:
  Ref<Renderer::AShader> m_shader_;
  Ref<Renderer::Buffer::AVertexArray> m_vertex_array_;
};

}  // namespace xuzy::UI
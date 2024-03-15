#include "pch.h"

#include "sandbox2D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::UI {

Sandbox2D::Sandbox2D()
    : ALayer("Sandbox2DLayer"), m_camera_controller_(1280.0f / 720.0f, true) {}

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::on_attach() {
  LOG(INFO) << "Sandbox2D OnAttach";
  m_check_board_texture_ =
      Renderer::ATexture2D::Create("../assets/textures/Checkerboard.png");
}

void Sandbox2D::on_detach() { LOG(INFO) << "Sandbox2D OnDetach"; }

void Sandbox2D::on_update(Renderer::Times::Timestep p_ts) {
  XUZY_PROFILE_FUNCTION();

  // Update
  {
    XUZY_PROFILE_SCOPE("CameraController::on_update");
    m_camera_controller_.on_update(p_ts);
  }

  // Render
  {
    XUZY_PROFILE_SCOPE("Renderer Prep");
    Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::RenderCommand::clear();
  }

  {
    XUZY_PROFILE_SCOPE("Renderer Draw");
    Renderer::Renderer2D::begin_scene(m_camera_controller_.get_camera());

    Renderer::Renderer2D::draw_rotated_quad(Maths::FVector2(-1.0f, 0.0f),
                                    Maths::FVector2(0.8f, 0.8f), -45.0f,
                                    Maths::FVector4(0.8f, 0.2f, 0.3f, 1.0f));

    Renderer::Renderer2D::draw_quad(Maths::FVector2(0.5f, -0.5f),
                                    Maths::FVector2(0.5f, 0.75f),
                                    Maths::FVector4(0.2f, 0.3f, 0.8f, 1.0f));

    Renderer::Renderer2D::draw_quad(Maths::FVector3(0.0f, 0.0f, -0.1f),
                                    Maths::FVector2(10.0f, 10.0f),
                                    m_check_board_texture_, 10.0f);

    Renderer::Renderer2D::end_scene();
  }
}

void Sandbox2D::on_event(Ref<Events::Event> event, bool& handled) {
  XUZY_PROFILE_FUNCTION();

  m_camera_controller_.on_event(event, handled);
}

void Sandbox2D::on_draw() {}

}  // namespace xuzy::UI
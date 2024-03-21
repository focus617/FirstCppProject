#include "pch.h"

#include "sandbox2D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::UI {

static const uint32_t s_map_width = 24;
static const char* s_map_tiles =
    "WWWWWWWWDDDDDWWWWWWWWWWW"
    "WWWWWWWWDDDDDWWWWWWWWWWW"
    "WWWWWWWDDDDDDDDWWWWWWWWW"
    "WWWWWWDDDDDDDDDDWWWWWWWW"
    "WWWWDDDDDDWDDDDDDDDWWWWW"
    "WWWDDDDDDWWWWDDDDDDDDWWW"
    "WWDDDDDDWWWWWWWDDDDDDDWW"
    "WWWDDTDDCDWWDDDDDDDDDWWW"
    "WWWDDDDDDDDDDDDDDDDDDWWW"
    "WWWWWDDDSDDDDDDDDWWWWWWW"
    "WWWWWWWDDDDDDDDWWWWWWWWW"
    "WWBWWWWWDDDDDWWWWWWWWWWW"
    "WWWWWWWWWDDDWWWWWWWWWWWW"
    "WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D()
    : ALayer("Sandbox2DLayer"), m_camera_controller_(1280.0f / 720.0f, true) {}

Sandbox2D::~Sandbox2D() {}

void Sandbox2D::on_attach() {
  LOG(INFO) << "Sandbox2D OnAttach";
  m_check_board_texture_ =
      Renderer::ATexture2D::Create("../assets/textures/Checkerboard.png");

  m_sprite_texture_ = Renderer::ATexture2D::Create(
      "../assets/demo/textures/RPGpack_sheet_2X.png");

  m_map_width_ = s_map_width;
  m_map_height = strlen(s_map_tiles) / s_map_width;

  s_texture_map_['W'] = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {11, 11}, {128, 128});
  s_texture_map_['D'] = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {6, 11}, {128, 128});
  s_texture_map_['T'] = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {0, 1}, {128, 128}, {1, 2});
  s_texture_map_['S'] = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {7, 6}, {128, 128});
  s_texture_map_['B'] = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {8, 2}, {128, 128});

  m_default_subtexture_ = Renderer::SubTexture2D::CreateFromCoords(
      m_sprite_texture_, {11, 11}, {128, 128});

  m_camera_controller_.set_zoom_level(7.0f);
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
  Renderer::Renderer2D::reset_stats();
  {
    XUZY_PROFILE_SCOPE("Renderer Prep");
    Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::RenderCommand::clear();
  }

  // static float rotation = 0.0f;
  // rotation += p_ts * 50.0f;

  {
    XUZY_PROFILE_SCOPE("Renderer Draw");
    // Renderer::Renderer2D::begin_scene(m_camera_controller_.get_camera());
    // // Background
    // Renderer::Renderer2D::draw_quad(Maths::FVector3(0.0f, 0.0f, -0.1f),
    //                                 Maths::FVector2(20.0f, 20.0f),
    //                                 m_check_board_texture_, 10.0f);

    // // Red Square 1
    // Renderer::Renderer2D::draw_rotated_quad(
    //     Maths::FVector2(1.0f, 0.0f), Maths::FVector2(0.8f, 0.8f), rotation,
    //     Maths::FVector4(0.8f, 0.2f, 0.3f, 1.0f));

    // // Red Square 2
    // Renderer::Renderer2D::draw_quad(Maths::FVector2(-1.0f, 0.0f),
    //                                 Maths::FVector2(0.8f, 0.8f),
    //                                 Maths::FVector4(0.8f, 0.2f, 0.3f, 1.0f));

    // // Blue Square
    // Renderer::Renderer2D::draw_quad(Maths::FVector2(0.5f, -0.5f),
    //                                 Maths::FVector2(0.5f, 0.75f),
    //                                 Maths::FVector4(0.2f, 0.3f, 0.8f, 1.0f));

    // // CheckBoard Square
    // Renderer::Renderer2D::draw_rotated_quad(Maths::FVector3(0.0f, 0.0f,
    // 0.0f),
    //                                         Maths::FVector2(1.0f, 1.0f), 45.0f,
    //                                         m_check_board_texture_, 20.0f);

    // Renderer::Renderer2D::end_scene();

    Renderer::Renderer2D::begin_scene(m_camera_controller_.get_camera());

    for (uint32_t y = 0; y < m_map_height; y++) {
      for (uint32_t x = 0; x < m_map_width_; x++) {
        char tile_type = s_map_tiles[x + y * s_map_width];

        Ref<Renderer::SubTexture2D> texture;
        if (s_texture_map_.find(tile_type) != s_texture_map_.end()) {
          texture = s_texture_map_[tile_type];
        } else
          texture = m_default_subtexture_;

        Renderer::Renderer2D::draw_quad(
            Maths::FVector3(x - m_map_width_ / 2.0f,
                            // flipping here since opengl coords is reversed
                            m_map_height / 2.0f - y, 0.5f),
            Maths::FVector2(1.0f, 1.0f), texture);
      }
    }

    Renderer::Renderer2D::end_scene();
  }
}

void Sandbox2D::on_event(Ref<Events::Event> event, bool& handled) {
  XUZY_PROFILE_FUNCTION();

  m_camera_controller_.on_event(event, handled);
}

void Sandbox2D::on_draw() {}

}  // namespace xuzy::UI
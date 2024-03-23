#include "pch.h"

#include "editor/panels/renderer2D_panel.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ui/imgui/imgui.h"
#include "ui/internal/converter.hpp"

namespace xuzy::UI::Panels {

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

Renderer2DPanel::Renderer2DPanel(
    const std::string& p_name, bool p_opened,
    const Settings::PanelWindowSettings& p_panelSettings)
    : PanelWindow(p_name, p_opened, p_panelSettings),
      m_camera_controller_(1280.0f / 720.0f, true) {
  LOG(INFO) << "Renderer2DPanel Created";

  Renderer::Buffer::FrameBufferSpecification fb_spec;
  fb_spec.width = 1280;
  fb_spec.height = 720;
  m_framebuffer_ = Renderer::Buffer::AFrameBuffer::Create(fb_spec);

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

Renderer2DPanel::~Renderer2DPanel() { LOG(INFO) << "Renderer2DPanel Deleted."; }

void Renderer2DPanel::on_update(Renderer::Times::Timestep p_ts) {
  XUZY_PROFILE_FUNCTION();

  // Update
  {
    XUZY_PROFILE_SCOPE("CameraController::on_update");
    if (is_focused()) m_camera_controller_.on_update(p_ts);
  }

  // Render
  Renderer::Renderer2D::reset_stats();
  {
    XUZY_PROFILE_SCOPE("Renderer Prep");

    m_framebuffer_->bind();
    Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::RenderCommand::clear();
  }

  {
    XUZY_PROFILE_SCOPE("Renderer Draw");

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
    m_framebuffer_->unbind();
  }
}

void Renderer2DPanel::_on_draw_impl() { PanelWindow::_on_draw_impl(); }

void Renderer2DPanel::on_event(Ref<Events::Event> event, bool& handled) {
  XUZY_PROFILE_FUNCTION();

  // Handle global event, e.g. WindowResize
  if (Events::EventId::WindowResize == event->get_event_id()) {
    LOG(INFO) << "Event: " << *event << std::endl;

    m_framebuffer_->resize((uint32_t)content_region_size.x,
                           (uint32_t)content_region_size.y);
  }

  m_camera_controller_.on_event(event, handled);
}

}  // namespace xuzy::UI::Panels
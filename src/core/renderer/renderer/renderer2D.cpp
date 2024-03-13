#include "renderer/renderer/renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::Renderer {

struct Renderer2D_Storage {
  Ref<Buffer::AVertexArray> quad_vertex_array;
  Ref<AShader> texture_shader;
  Ref<ATexture2D> white_texture;
};

static Renderer2D_Storage* s_data;

void Renderer2D::init() {
  s_data = new Renderer2D_Storage();
  s_data->quad_vertex_array = Buffer::AVertexArray::Create();

  float square_vertices[5 * 4] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom-left corner
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom-right corner
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top-right corner
      -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top-left corner
  };
  Ref<Buffer::AVertexBuffer> square_vertex_buffer =
      Buffer::AVertexBuffer::Create(square_vertices, sizeof(square_vertices));

  Buffer::BufferLayout square_layout = {
      {Buffer::ShaderDataType::FVec3, "a_Position"},
      {Buffer::ShaderDataType::FVec2, "a_TexCoord"}};
  square_vertex_buffer->set_layout(square_layout);
  s_data->quad_vertex_array->add_vertex_buffer(square_vertex_buffer);

  unsigned int square_indices[6] = {0, 1, 2, 2, 3, 0};
  Ref<Buffer::AIndexBuffer> square_index_buffer = Buffer::AIndexBuffer::Create(
      square_indices, sizeof(square_indices) / sizeof(uint32_t));
  s_data->quad_vertex_array->set_index_buffer(square_index_buffer);

  s_data->white_texture = ATexture2D::Create(1, 1);
  uint32_t white_texture_data = 0xffffffff;
  s_data->white_texture->set_data(&white_texture_data, sizeof(uint32_t));

  s_data->texture_shader =
      AShader::Create("../assets/shaders/opengl/texture_color.glsl");
  s_data->texture_shader->bind();
  s_data->texture_shader->set_int("u_Texture", 0);
}

void Renderer2D::shutdown() { delete s_data; }

void Renderer2D::on_window_resize(uint32_t p_width, uint32_t p_height) {
  RenderCommand::set_viewport(0, 0, p_width, p_height);
}

void Renderer2D::begin_scene(Camera::OrthographicCamera& p_camera) {
  s_data->texture_shader->bind();
  s_data->texture_shader->set_mat4("u_ViewProjection",
                                   p_camera.get_view_projection_matrix());
}

void Renderer2D::end_scene() {}

void Renderer2D::draw_quad(const Maths::FVector2& p_position,
                           const Maths::FVector2& p_size,
                           const Maths::FVector4& p_color) {
  draw_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size, p_color);
}

void Renderer2D::draw_quad(const Maths::FVector3& p_position,
                           const Maths::FVector2& p_size,
                           const Maths::FVector4& p_color) {
  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  s_data->texture_shader->set_fvec4("u_Color", p_color);
  s_data->texture_shader->set_mat4("u_Transform", transform);

  s_data->white_texture->bind();
  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

void Renderer2D::draw_quad(const Maths::FVector2& p_position,
                           const Maths::FVector2& p_size,
                           const Ref<ATexture2D>& p_texture) {
  draw_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size,
            p_texture);
}

void Renderer2D::draw_quad(const Maths::FVector3& p_position,
                           const Maths::FVector2& p_size,
                           const Ref<ATexture2D>& p_texture) {
  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  s_data->texture_shader->set_fvec4("u_Color", Maths::FVector4::One);
  s_data->texture_shader->set_mat4("u_Transform", transform);

  p_texture->bind();
  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

}  // namespace xuzy::Renderer
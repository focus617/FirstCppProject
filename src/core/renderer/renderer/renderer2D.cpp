#include "renderer/renderer/renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::Renderer {

struct QuadVertex {
  Maths::FVector3 position;
  Maths::FVector4 color;
  Maths::FVector2 tex_coord;
  float tex_index;
  float tiling_factor;
};

struct Renderer2D_Data {
  const uint32_t MaxQuads = 10000;
  const uint32_t MaxVertices = MaxQuads * 4;
  const uint32_t MaxIndices = MaxQuads * 6;
  static const uint32_t MaxTextureSlots = 32;  // TODO: RenderCaps

  Ref<Buffer::AVertexArray> quad_vertex_array;
  Ref<Buffer::AVertexBuffer> quad_vertex_buffer;
  Ref<AShader> texture_shader;
  Ref<ATexture2D> white_texture;

  uint32_t quad_index_count = 0;
  QuadVertex* quad_vertex_buffer_base = nullptr;
  QuadVertex* quad_vertex_buffer_ptr = nullptr;

  std::array<Ref<ATexture2D>, MaxTextureSlots> texture_slots;
  uint32_t texture_slot_index = 1;  // Next free texture slot
                                    // Note: slot 0 used by white texture

  glm::vec4 quad_vertex_positions[4];
};

static Renderer2D_Data* s_data;

void Renderer2D::init() {
  XUZY_PROFILE_FUNCTION();

  s_data = new Renderer2D_Data();

  s_data->quad_vertex_array = Buffer::AVertexArray::Create();

  s_data->quad_vertex_buffer =
      Buffer::AVertexBuffer::Create(s_data->MaxVertices * sizeof(QuadVertex));
  s_data->quad_vertex_buffer->set_layout(
      {{Buffer::ShaderDataType::FVec3, "a_Position"},
       {Buffer::ShaderDataType::FVec4, "a_Color"},
       {Buffer::ShaderDataType::FVec2, "a_TexCoord"},
       {Buffer::ShaderDataType::Float, "a_TexIndex"},
       {Buffer::ShaderDataType::Float, "a_TilingFactor"}});
  s_data->quad_vertex_array->add_vertex_buffer(s_data->quad_vertex_buffer);

  s_data->quad_vertex_buffer_base = new QuadVertex[s_data->MaxVertices];

  uint32_t* quad_indices = new uint32_t[s_data->MaxIndices];
  uint32_t offset = 0;
  for (uint32_t i = 0; i < s_data->MaxIndices; i += 6) {
    quad_indices[i + 0] = offset + 0;
    quad_indices[i + 1] = offset + 1;
    quad_indices[i + 2] = offset + 2;

    quad_indices[i + 3] = offset + 2;
    quad_indices[i + 4] = offset + 3;
    quad_indices[i + 5] = offset + 0;

    offset += 4;
  }

  Ref<Buffer::AIndexBuffer> quad_index_buffer =
      Buffer::AIndexBuffer::Create(quad_indices, s_data->MaxIndices);
  s_data->quad_vertex_array->set_index_buffer(quad_index_buffer);
  delete[] quad_indices;

  s_data->white_texture = ATexture2D::Create(1, 1);
  uint32_t white_texture_data = 0xffffffff;
  s_data->white_texture->set_data(&white_texture_data, sizeof(uint32_t));

  // Set texture slot #0 to default WhiteTexture
  s_data->texture_slots[0] = s_data->white_texture;

  int32_t samplers[s_data->MaxTextureSlots];
  for (uint32_t i = 0; i < s_data->MaxTextureSlots; i++) samplers[i] = i;

  s_data->texture_shader =
      AShader::Create("../assets/shaders/opengl/texture2D.glsl");
  s_data->texture_shader->bind();
  s_data->texture_shader->set_int_array("u_Textures", samplers,
                                        s_data->MaxTextureSlots);

  s_data->quad_vertex_positions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  s_data->quad_vertex_positions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  s_data->quad_vertex_positions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  s_data->quad_vertex_positions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::shutdown() {
  XUZY_PROFILE_FUNCTION();
  if (nullptr != s_data->quad_vertex_buffer_base) {
    delete[] s_data->quad_vertex_buffer_base;
    s_data->quad_vertex_buffer_base = nullptr;
  }
  delete s_data;
}

void Renderer2D::on_window_resize(uint32_t p_width, uint32_t p_height) {
  XUZY_PROFILE_FUNCTION();

  RenderCommand::set_viewport(0, 0, p_width, p_height);
}

void Renderer2D::begin_scene(Camera::OrthographicCamera& p_camera) {
  XUZY_PROFILE_FUNCTION();

  s_data->texture_shader->bind();
  s_data->texture_shader->set_mat4("u_ViewProjection",
                                   p_camera.get_view_projection_matrix());

  s_data->quad_index_count = 0;
  s_data->quad_vertex_buffer_ptr = s_data->quad_vertex_buffer_base;
  s_data->texture_slot_index = 1;
}

void Renderer2D::end_scene() {
  XUZY_PROFILE_FUNCTION();

  uint32_t data_size = (uint8_t*)s_data->quad_vertex_buffer_ptr -
                       (uint8_t*)s_data->quad_vertex_buffer_base;
  s_data->quad_vertex_buffer->set_data(s_data->quad_vertex_buffer_base,
                                       data_size);
  flush();
}

void Renderer2D::flush() {
  // Bind textures
  for (uint32_t i = 0; i < s_data->texture_slot_index; i++)
    s_data->texture_slots[i]->bind(i);

  RenderCommand::draw_indexed(s_data->quad_vertex_array,
                              s_data->quad_index_count);
}

void Renderer2D::draw_quad(const Maths::FVector2& p_position,
                           const Maths::FVector2& p_size,
                           const Maths::FVector4& p_color) {
  XUZY_PROFILE_FUNCTION();

  draw_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size, p_color);
}

void Renderer2D::draw_quad(const Maths::FVector3& p_position,
                           const Maths::FVector2& p_size,
                           const Maths::FVector4& p_color) {
  XUZY_PROFILE_FUNCTION();

  const float texture_index = 0.0f;  // index of WhiteTexture
  const float tiling_factor = 1.0f;

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  glm::vec4 vertex_positions[4];
  for (uint32_t i = 0; i < 4; i++) {
    vertex_positions[i] = transform * s_data->quad_vertex_positions[i];
  }

  // Bottom-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[0].x, vertex_positions[0].y, vertex_positions[0].z};
  s_data->quad_vertex_buffer_ptr->color = p_color;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Bottom-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[1].x, vertex_positions[1].y, vertex_positions[1].z};
  s_data->quad_vertex_buffer_ptr->color = p_color;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[2].x, vertex_positions[2].y, vertex_positions[2].z};
  s_data->quad_vertex_buffer_ptr->color = p_color;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[3].x, vertex_positions[3].y, vertex_positions[3].z};
  s_data->quad_vertex_buffer_ptr->color = p_color;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = tiling_factor;
  s_data->quad_vertex_buffer_ptr++;

  s_data->quad_index_count += 6;

  // s_data->texture_shader->set_fvec4("u_Color", p_color);
  // s_data->texture_shader->set_float("u_TilingFactor", 1.0f);
  // s_data->texture_shader->set_mat4("u_Transform", transform);

  // s_data->white_texture->bind();
  // s_data->quad_vertex_array->bind();
  // RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

void Renderer2D::draw_quad(const Maths::FVector2& p_position,
                           const Maths::FVector2& p_size,
                           const Ref<ATexture2D>& p_texture,
                           float p_tiling_factor,
                           const Maths::FVector4& p_tint_color) {
  XUZY_PROFILE_FUNCTION();

  draw_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size,
            p_texture, p_tiling_factor, p_tint_color);
}

void Renderer2D::draw_quad(const Maths::FVector3& p_position,
                           const Maths::FVector2& p_size,
                           const Ref<ATexture2D>& p_texture,
                           float p_tiling_factor,
                           const Maths::FVector4& p_tint_color) {
  XUZY_PROFILE_FUNCTION();

  const Maths::FVector4 WHITE = Maths::FVector4::One;

  // Seach texture index of Vertex attribute 'a_TexIndex'
  float texture_index = 0.0f;

  // First check if the texture has already occupied one of texture slot
  for (uint32_t i = 1; i < s_data->texture_slot_index; i++) {
    if (*s_data->texture_slots[i].get() == *p_texture.get()) {
      texture_index = (float)i;
      break;
    }
  }
  // If the texture is a new one, assign next free texture slot to it.
  if (texture_index == 0.0f) {
    texture_index = (float)s_data->texture_slot_index;
    s_data->texture_slots[s_data->texture_slot_index] = p_texture;
    s_data->texture_slot_index++;
  }

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  glm::vec4 vertex_positions[4];
  for (uint32_t i = 0; i < 4; i++) {
    vertex_positions[i] = transform * s_data->quad_vertex_positions[i];
  }

  // Bottom-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[0].x, vertex_positions[0].y, vertex_positions[0].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Bottom-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[1].x, vertex_positions[1].y, vertex_positions[1].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[2].x, vertex_positions[2].y, vertex_positions[2].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[3].x, vertex_positions[3].y, vertex_positions[3].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;

  s_data->quad_index_count += 6;

  // s_data->texture_shader->set_fvec4("u_Color", p_tint_color);
  // s_data->texture_shader->set_float("u_TilingFactor", p_tiling_factor);
  // s_data->texture_shader->set_mat4("u_Transform", transform);

  // p_texture->bind();
  // s_data->quad_vertex_array->bind();
  // RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

void Renderer2D::draw_rotated_quad(const Maths::FVector2& p_position,
                                   const Maths::FVector2& p_size,
                                   float p_rotation,
                                   const Maths::FVector4& p_color) {
  XUZY_PROFILE_FUNCTION();

  draw_rotated_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size,
                    p_rotation, p_color);
}

void Renderer2D::draw_rotated_quad(const Maths::FVector3& p_position,
                                   const Maths::FVector2& p_size,
                                   float p_rotation,
                                   const Maths::FVector4& p_color) {
  XUZY_PROFILE_FUNCTION();

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::rotate(glm::mat4(1.0f), glm::radians(p_rotation),
                  {0.0f, 0.0f, 1.0f}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  s_data->texture_shader->set_fvec4("u_Color", p_color);
  s_data->texture_shader->set_float("u_TilingFactor", 1.0f);
  s_data->texture_shader->set_mat4("u_Transform", transform);

  s_data->white_texture->bind();
  s_data->quad_vertex_array->bind();
  RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

void Renderer2D::draw_rotated_quad(const Maths::FVector2& p_position,
                                   const Maths::FVector2& p_size,
                                   float p_rotation,
                                   const Ref<ATexture2D>& p_texture,
                                   float p_tiling_factor,
                                   const Maths::FVector4& p_tint_color) {
  XUZY_PROFILE_FUNCTION();

  draw_rotated_quad(Maths::FVector3(p_position.x, p_position.y, 0.0f), p_size,
                    p_rotation, p_texture, p_tiling_factor, p_tint_color);
}

void Renderer2D::draw_rotated_quad(const Maths::FVector3& p_position,
                                   const Maths::FVector2& p_size,
                                   float p_rotation,
                                   const Ref<ATexture2D>& p_texture,
                                   float p_tiling_factor,
                                   const Maths::FVector4& p_tint_color) {
  XUZY_PROFILE_FUNCTION();

  const Maths::FVector4 WHITE = Maths::FVector4::One;

  // Seach texture index of Vertex attribute 'a_TexIndex'
  float texture_index = 0.0f;

  // First check if the texture has already occupied one of texture slot
  for (uint32_t i = 1; i < s_data->texture_slot_index; i++) {
    if (*s_data->texture_slots[i].get() == *p_texture.get()) {
      texture_index = (float)i;
      break;
    }
  }
  // If the texture is a new one, assign next free texture slot to it.
  if (texture_index == 0.0f) {
    texture_index = (float)s_data->texture_slot_index;
    s_data->texture_slots[s_data->texture_slot_index] = p_texture;
    s_data->texture_slot_index++;
  }

  glm::mat4 transform =
      glm::translate(glm::mat4(1.0f),
                     {p_position.x, p_position.y, p_position.z}) *
      glm::rotate(glm::mat4(1.0f), glm::radians(p_rotation),
                  {0.0f, 0.0f, 1.0f}) *
      glm::scale(glm::mat4(1.0f), {p_size.x, p_size.y, 1.0f});

  glm::vec4 vertex_positions[4];
  for (uint32_t i = 0; i < 4; i++) {
    vertex_positions[i] = transform * s_data->quad_vertex_positions[i];
  }

  // Bottom-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[0].x, vertex_positions[0].y, vertex_positions[0].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Bottom-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[1].x, vertex_positions[1].y, vertex_positions[1].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 0.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Right
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[2].x, vertex_positions[2].y, vertex_positions[2].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {1.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;
  // Top-Left
  s_data->quad_vertex_buffer_ptr->position = {
      vertex_positions[3].x, vertex_positions[3].y, vertex_positions[3].z};
  s_data->quad_vertex_buffer_ptr->color = WHITE;
  s_data->quad_vertex_buffer_ptr->tex_coord = {0.0f, 1.0f};
  s_data->quad_vertex_buffer_ptr->tex_index = texture_index;
  s_data->quad_vertex_buffer_ptr->tiling_factor = p_tiling_factor;
  s_data->quad_vertex_buffer_ptr++;

  s_data->quad_index_count += 6;

  // s_data->texture_shader->set_fvec4("u_Color", p_tint_color);
  // s_data->texture_shader->set_float("u_TilingFactor", p_tiling_factor);
  // s_data->texture_shader->set_mat4("u_Transform", transform);

  // p_texture->bind();
  // s_data->quad_vertex_array->bind();
  // RenderCommand::draw_indexed(s_data->quad_vertex_array);
}

}  // namespace xuzy::Renderer
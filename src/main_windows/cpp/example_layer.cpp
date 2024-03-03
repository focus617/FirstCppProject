#include "example_layer.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace xuzy::UI {

ExampleLayer::ExampleLayer()
    : ALayer("ExampleLayer"),
      m_camera_(-1.6f, 1.6f, -0.9f, 0.9f),
      m_camera_position_(0.0f),
      m_camera_rotation_(0.0f) {
  /******* Color Triangle ***********/
  m_triangle_vertex_array_ = Renderer::Buffer::AVertexArray::Create();

  float triangle_vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                                    0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                                    0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

  Ref<Renderer::Buffer::AVertexBuffer> triangle_vertex_buffer =
      Renderer::Buffer::AVertexBuffer::Create(triangle_vertices,
                                              sizeof(triangle_vertices));

  Renderer::Buffer::BufferLayout triangle_layout = {
      {Renderer::Buffer::ShaderDataType::FVec3, "a_Position"},
      {Renderer::Buffer::ShaderDataType::FVec4, "a_Color"}};
  triangle_vertex_buffer->set_layout(triangle_layout);
  m_triangle_vertex_array_->add_vertex_buffer(triangle_vertex_buffer);

  unsigned int triangle_indices[3] = {0, 1, 2};
  Ref<Renderer::Buffer::AIndexBuffer> triangle_index_buffer =
      Renderer::Buffer::AIndexBuffer::Create(
          triangle_indices, sizeof(triangle_indices) / sizeof(uint32_t));
  m_triangle_vertex_array_->set_index_buffer(triangle_index_buffer);

  std::string triangle_vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

  std::string triangle_fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

  m_triangle_shader_ =
      Renderer::AShader::Create(triangle_vertex_src, triangle_fragment_src);

  /******* Blue Square ***********/
  m_square_vertex_array_ = Renderer::Buffer::AVertexArray::Create();

  float square_vertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

  Ref<Renderer::Buffer::AVertexBuffer> square_vertex_buffer =
      Renderer::Buffer::AVertexBuffer::Create(square_vertices,
                                              sizeof(square_vertices));

  Renderer::Buffer::BufferLayout square_layout = {
      {Renderer::Buffer::ShaderDataType::FVec3, "a_Position"}};
  square_vertex_buffer->set_layout(square_layout);
  m_square_vertex_array_->add_vertex_buffer(square_vertex_buffer);

  unsigned int square_indices[6] = {0, 1, 2, 2, 3, 0};
  Ref<Renderer::Buffer::AIndexBuffer> square_index_buffer =
      Renderer::Buffer::AIndexBuffer::Create(
          square_indices, sizeof(square_indices) / sizeof(uint32_t));
  m_square_vertex_array_->set_index_buffer(square_index_buffer);

  std::string square_vertex_src = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

  std::string square_fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

      uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

  m_square_shader_ =
      Renderer::AShader::Create(square_vertex_src, square_fragment_src);
}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::on_update(Renderer::Times::Timestep p_ts) {
  if (Window::Inputs::InputManager::is_key_pressed(Window::Inputs::Key::Left)) {
    m_camera_position_.x += m_camera_move_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::Right)) {
    m_camera_position_.x -= m_camera_move_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::Up)) {
    m_camera_position_.y -= m_camera_move_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::Down)) {
    m_camera_position_.y += m_camera_move_speed_ * p_ts;
  }

  if (Window::Inputs::InputManager::is_key_pressed(Window::Inputs::Key::A)) {
    m_camera_rotation_ += m_camera_rotate_speed_ * p_ts;
  } else if (Window::Inputs::InputManager::is_key_pressed(
                 Window::Inputs::Key::Z)) {
    m_camera_rotation_ -= m_camera_rotate_speed_ * p_ts;
  }

  Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
  Renderer::RenderCommand::clear();

  m_camera_.set_position(m_camera_position_);
  m_camera_.set_rotation(m_camera_rotation_);

  Renderer::Renderer::begin_scene(m_camera_);

  m_square_shader_->bind();
  m_square_shader_->set_vec3("u_Color", m_square_color_);

  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
      Renderer::Renderer::submit(m_square_shader_, m_square_vertex_array_,
                                 transform);
    }
  }

  Renderer::Renderer::submit(m_triangle_shader_, m_triangle_vertex_array_);

  Renderer::Renderer::end_scene();
}
void ExampleLayer::on_event(Ref<Events::Event> event, bool& handled) {}

void ExampleLayer::on_draw() {}

}  // namespace xuzy::UI
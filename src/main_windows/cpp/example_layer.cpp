#include "example_layer.hpp"

#include "renderer/buffers/AIndex_buffer.hpp"
#include "renderer/buffers/AVertex_buffer.hpp"
#include "renderer/buffers/buffer_layout.hpp"
#include "renderer/core/render_command.hpp"
#include "renderer/core/renderer.hpp"
#include "renderer/resources/shader/AShader.hpp"

namespace xuzy::UI {

ExampleLayer::ExampleLayer() : ALayer("ExampleLayer") {
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position - 0.5, 1.0);	
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

  float square_vertices[3 * 4] = {-0.75f, -0.75f, 0.0f, 0.75f,  -0.75f, 0.0f,
                                  0.75f,  0.75f,  0.0f, -0.75f, 0.75f,  0.0f};

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

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

  std::string square_fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

  m_square_shader_ =
      Renderer::AShader::Create(square_vertex_src, square_fragment_src);
}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::on_update() {
  Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
  Renderer::RenderCommand::clear();

  Renderer::Renderer::begin_scene();

  Renderer::Renderer::submit(m_square_shader_, m_square_vertex_array_);
  Renderer::Renderer::submit(m_triangle_shader_, m_triangle_vertex_array_);

  Renderer::Renderer::end_scene();
}
void ExampleLayer::on_event(Ref<Events::Event> event, bool& handled) {}

void ExampleLayer::on_draw() {}

}  // namespace xuzy::UI
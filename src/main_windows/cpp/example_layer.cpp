#include "example_layer.hpp"

#include "renderer/buffers/AIndex_buffer.hpp"
#include "renderer/buffers/AVertex_buffer.hpp"
#include "renderer/buffers/buffer_layout.hpp"
#include "renderer/core/render_command.hpp"
#include "renderer/core/renderer.hpp"
#include "renderer/resources/shader/AShader.hpp"

namespace xuzy::UI {

ExampleLayer::ExampleLayer() : ALayer("ExampleLayer") {
  m_vertex_array_ = Renderer::Buffer::AVertexArray::Create();

  float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                           0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                           0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f};

  Ref<Renderer::Buffer::AVertexBuffer> vertex_buffer =
      Renderer::Buffer::AVertexBuffer::Create(vertices, sizeof(vertices));

  Renderer::Buffer::BufferLayout layout = {
      {Renderer::Buffer::ShaderDataType::FVec3, "a_Position"},
      {Renderer::Buffer::ShaderDataType::FVec4, "a_Color"}};
  vertex_buffer->set_layout(layout);
  m_vertex_array_->add_vertex_buffer(vertex_buffer);

  uint32_t indices[3] = {0, 1, 2};
  Ref<Renderer::Buffer::AIndexBuffer> index_buffer =
      Renderer::Buffer::AIndexBuffer::Create(
          indices, sizeof(indices) / sizeof(uint32_t));
  m_vertex_array_->set_index_buffer(index_buffer);

  std::string vertex_src = R"(
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

  std::string fragment_src = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

  m_shader_ =
      Renderer::AShader::Create("VertexPosColor", vertex_src, fragment_src);
}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::on_update() {}
void ExampleLayer::on_event(Ref<Events::Event> event, bool& handled) {}

void ExampleLayer::on_draw() {
  Renderer::RenderCommand::set_clear_color(0.1f, 0.1f, 0.1f, 1.0f);
  Renderer::RenderCommand::clear();

  Renderer::Renderer::begin_scene();
  Renderer::Renderer::submit(m_shader_, m_vertex_array_);
  Renderer::Renderer::end_scene();
}

}  // namespace xuzy::UI
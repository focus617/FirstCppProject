#include "renderer/core/render_command.hpp"

namespace xuzy::Renderer {

    Scope<ARendererAPI> RenderCommand::s_renderer_api = ARendererAPI::Create();

}  // namespace xuzy::Renderer
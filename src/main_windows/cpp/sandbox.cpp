#include "editor/core/editor.hpp"
#include "example_layer.hpp"
#include "sandbox2D.hpp"

using namespace xuzy;

class Sandbox : public Editor::Editor {
 public:
  Sandbox(const std::string& t_app_name, const std::string& t_version)
      : Editor::Editor{std::move(t_app_name), std::move(t_version)} {
    // Ref<UI::ExampleLayer> example_layer = CreateRef<UI::ExampleLayer>();
    // push_layer(example_layer);

    // Ref<UI::Sandbox2D> sandbox_2d_layer = CreateRef<UI::Sandbox2D>();
    // push_layer(sandbox_2d_layer);
  }

  ~Sandbox() {}
};

namespace xuzy {

const std::string APP_NAME{"Windows-App"};
const std::string VERSION{"0.0.1"};

App* CreateApplication() {
  App* app = new Sandbox(APP_NAME, VERSION);
  return app;
}

}  // namespace xuzy
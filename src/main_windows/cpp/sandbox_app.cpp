#include "app/window_app.hpp"

class Sandbox : public xuzy::WindowApp {
 public:
  Sandbox(const std::string& t_app_name, const std::string& t_version)
      : WindowApp{std::move(t_app_name), std::move(t_version)} {}

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
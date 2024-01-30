#include <core.h>

#include "app/app.hpp"
#include "args.hpp"
#include "restful/http/restful_server.hpp"

const std::string APP_NAME{"Restful-Server"};
const std::string VERSION{"0.0.1"};

int main(int argc, char* argv[]) {
  xuzy::App* app = new http::RestfulServer(APP_NAME);

  app->set_cli_parser(new CLI_Parser());

  xuzy::App::main(argc, argv, VERSION, app);

  delete app;

  return EXIT_SUCCESS;
}

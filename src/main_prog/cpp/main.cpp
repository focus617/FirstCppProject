#include <core.h>

#include "app/app.hpp"
#include "args.hpp"
#include "restful/http/restful_server.hpp"

extern xuzy::App* xuzy::CreateApplication();

int main(int argc, char* argv[]) {
  // defined in specific application, e.g. restful_server.cpp
  xuzy::App* app = xuzy::CreateApplication();

  app->set_cli_parser(new CLI_Parser());

  xuzy::App::main(argc, argv, app);

  delete app;

  return EXIT_SUCCESS;
}

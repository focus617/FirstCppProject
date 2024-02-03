#include <core.h>

#include "app/app.hpp"
#include "args.hpp"
#include "restful/http/restful_server.hpp"

using namespace xuzy;

extern App* xuzy::CreateApplication();

void init_logger(const char* argv0);
void close_logger();

int main(int argc, char* argv[]) {
  // Initialize Google’s logging library.
  init_logger(argv[0]);

  // defined in specific application, e.g. restful_server.cpp
  App* app = CreateApplication();

  app->set_cli_parser(new CLI_Parser());

  app->main(argc, argv);

  // Cleanup
  delete app;
  close_logger();

  return EXIT_SUCCESS;
}


void init_logger(const char* argv0) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv0);
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
}

void close_logger() {
  // Shutdown Google’s logging system.
  google::ShutdownGoogleLogging();
}

#include <core.h>

#include "app/app.hpp"
#include "tools/file_system/path_parser.hpp"

extern xuzy::App* xuzy::CreateApplication();

void update_working_directory(const std::string& p_executable_path);
void init_logger(const char* argv0);
void close_logger();

int main(int argc, char* argv[]) {
  // Initialize Google’s logging library.
  init_logger(argv[0]);

  // defined in specific application, e.g. restful_server.cpp
  xuzy::App* app = xuzy::CreateApplication();

  app->set_cli_parser(new CLI_Parser());

  app->main(argc, argv);

  // Cleanup
  delete app;
  close_logger();

  return EXIT_SUCCESS;
}

/**
 * @brief When launching from a executable file, we should consider the
 * executable path as the current working directory.
 * @param p_executablePath
 */
void update_working_directory(const std::string& p_executable_path) {
  std::filesystem::current_path(
      xuzy::Tools::FileSystem::PathParser::get_containing_folder(
          p_executable_path));
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
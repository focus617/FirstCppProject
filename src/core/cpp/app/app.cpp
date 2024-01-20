
#include "app/app.hpp"

#include <glog/logging.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "exception.hpp"

using namespace xuzy;

App::App(std::string t_app_name)
    : m_app_name{t_app_name}, p_cli_parser{nullptr} {}

App::~App() {
  if (p_cli_parser) {
    delete p_cli_parser;
  }
}

void App::version_check(int argc, char* argv[], const std::string& version) {
  LOG(INFO) << m_app_name << " version: " << version << std::endl;
}

void App::dumpError(std::string error) {
  LOG(ERROR) << m_app_name << " error: " << error << std::endl;
}

void App::load_configuration_from_file(const std::string& filename) {
  if (!std::filesystem::exists(filename)) {
    throw xuzy::FileNotFoundException(filename + " not exist.");
  }

  LOG(INFO) << m_app_name << ": Load configuration from " << filename;
  std::ifstream config_file(filename.c_str());

  // Make sure the config is open
  if (!config_file.is_open()) {
    throw xuzy::OpenFileException("Failed to open " + filename);
  }
  if (config_file.std::ios::eof()) {
    throw xuzy::FileNotReadyException(filename + " is empty");
  }

  config_file >> m_conf_;
}

void App::setup() {
  // Setup configuration
  std::string config_filename = m_app_name + ".json";
  load_configuration_from_file(config_filename);

  if (m_conf_.empty()) {
    LOG(WARNING) << "Failure during config: empty configuration file."
                 << std::endl;
  }
}

void App::init_logger(const char* app) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(app);
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
}

// Command line parser
void App::set_cli_parser(ArgsParser* p_parser) {
  LOG(INFO) << m_app_name << ": Setup CLI parser" << std::endl;

  p_cli_parser = p_parser;
}

void App::main(int argc, char* argv[], const std::string& version,
               App* app = nullptr) {
  init_logger(argv[0]);

  if (app) {
    // Show outself, then run
    try {
      app->version_check(argc, argv, version);

      if (app->p_cli_parser) {
        app->p_cli_parser->parse_commandline(argc, argv);
      }

      // Call the subclass with the configuration
      app->setup();

      // Run forever
      app->run();
    } catch (xuzy::Exception& e) {
      app->dumpError(e.displayText());
    } catch (std::exception& e) {
      app->dumpError(e.what());
    } catch (std::string s) {
      app->dumpError(s);
    } catch (const char* s) {
      app->dumpError(s);
    } catch (...) {
      app->dumpError("Unknown");
    }
  }

  // Cleanup
  if (app) {
    delete app;
  }
}
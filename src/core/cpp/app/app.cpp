
#include "pch.h"
#include "app/app.hpp"

#include <glog/logging.h>

#include <filesystem>

#include "app/app_impl.hpp"


namespace xuzy {

internal::AppImpl* App::p_impl_ = internal::AppImpl::GetInstance();

App::App(const std::string& t_app_name, const std::string& t_version)
    : m_app_name_{t_app_name}, m_version_{t_version}, p_cli_parser_{nullptr} {}

App::~App() {
  if (nullptr != p_cli_parser_) {
    delete p_cli_parser_;
  }
}

void App::init_logger() {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(m_app_name_.c_str());
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
}

void App::close_logger() {
  // Shutdown Google’s logging system.
  google::ShutdownGoogleLogging();
}

void App::version_check(int argc, char* argv[]) {
  LOG(INFO) << m_app_name_ << " version: " << m_version_ << std::endl;
}

void App::dumpError(std::string error) {
  LOG(ERROR) << m_app_name_ << " error: " << error << std::endl;
}

void App::load_conf(const std::string& filename) {
  // Make sure the config file is exist
  if (!std::filesystem::exists(filename)) {
    throw xuzy::FileNotFoundException(filename + " not exist.");
  }

  LOG(INFO) << m_app_name_ << ": Load configuration from " << filename;
  App::GetImpl()->load_conf_from_file(filename, m_conf_);
}

void App::setup() {
  // Setup configuration
  std::string config_filename = m_app_name_ + ".json";
  load_conf(config_filename);

  if (m_conf_.empty()) {
    LOG(WARNING) << "Failure during config: empty configuration file."
                 << std::endl;
  }
}

// Command line parser
void App::set_cli_parser(ArgsParser* p_parser) { p_cli_parser_ = p_parser; }

void App::main(int argc, char* argv[], App* app) {
  XUZY_CHECK_(argc > 0) << "Invalid argc (value: " << argc << ").";
  XUZY_CHECK_(nullptr != app) << "Empty application";

  app->init_logger();

  // We don't want to run the initialization code twice.
  if (App::GetImpl()->is_initialized()) return;

  App::GetImpl()->init(argc, argv);

  // Show outself, then run
  try {
    app->version_check(argc, argv);

    if (app->p_cli_parser_) {
      app->p_cli_parser_->parse_commandline(argc, argv);
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

  app->close_logger();

  // Cleanup
  delete app;
}

}  // namespace xuzy
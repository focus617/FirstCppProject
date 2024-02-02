#include "pch.h"

#include "app/app.hpp"

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

void App::main(int argc, char* argv[]) {
  XUZY_CHECK_(argc > 0) << "Invalid argc (value: " << argc << ").";

  // We don't want to run the initialization code twice.
  if (App::GetImpl()->is_initialized()) return;

  App::GetImpl()->init(argc, argv);

  // Show outself, then run
  try {
    version_check(argc, argv);

    if (p_cli_parser_) {
      p_cli_parser_->parse_commandline(argc, argv);
    }

    // Call the derived class to setup configuration
    setup();

    // Launch threads in derived class
    launch_tasks();

    // Run forever
    main_loop();

  } catch (xuzy::Exception& e) {
    dumpError(e.displayText());
  } catch (std::exception& e) {
    dumpError(e.what());
  } catch (std::string s) {
    dumpError(s);
  } catch (const char* s) {
    dumpError(s);
  } catch (...) {
    dumpError("Unknown");
  }
}

}  // namespace xuzy
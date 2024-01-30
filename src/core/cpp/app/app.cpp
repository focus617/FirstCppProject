
#include "app/app.hpp"

#include <glog/logging.h>

#include <filesystem>

#include "app/app_impl.hpp"

namespace xuzy {

internal::AppImpl* App::p_impl_ = internal::AppImpl::GetInstance();

App::App(std::string t_app_name)
    : m_app_name_{t_app_name}, p_cli_parser_{nullptr} {}

App::~App() {
  if (nullptr != p_cli_parser_) {
    delete p_cli_parser_;
  }
}

void App::version_check(int argc, char* argv[], const std::string& version) {
  LOG(INFO) << m_app_name_ << " version: " << version << std::endl;
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

void App::init_logger(const char* app) {
  // Initialize Google’s logging library.
  google::InitGoogleLogging(app);
  // Log both to log file and stderr
  FLAGS_alsologtostderr = true;
}

// Command line parser
void App::set_cli_parser(ArgsParser* p_parser) { p_cli_parser_ = p_parser; }

void App::main(int argc, char* argv[], const std::string& version,
               App* app = nullptr) {
  if (argc <= 0) return;

  init_logger(argv[0]);

  // We don't want to run the initialization code twice.
  if (App::GetImpl()->is_initialized()) return;

  App::GetImpl()->init(argc, argv);

  if (app) {
    // Show outself, then run
    try {
      app->version_check(argc, argv, version);

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
  }

  // Cleanup
  if (app) {
    delete app;
  }
}

}  // namespace xuzy
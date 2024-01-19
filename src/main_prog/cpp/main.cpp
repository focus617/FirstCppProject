#include <glog/logging.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "app/app.hpp"
#include "args.hpp"
#include "config.h"
#include "logger.h"
#include "restful.hpp"
#include "xuzy_math.h"

using xuzy::Logger;

void test_logger();
// extern int test_httplib_client();
// extern void thread_trial();
// extern void thread_active_object();

const std::string VERSION{"0.0.1"};
const std::string APP_NAME{"Restful-Server"};

int main(int argc, char* argv[]) {
  xuzy::App* app = new http::Restful(APP_NAME);

  app->set_cli_parser(new CLI_Parser());

  xuzy::App::main(argc, argv, VERSION, app);

  /*
      test_logger();

      thread_trial();
      thread_active_object();
  */

  return EXIT_SUCCESS;
}

void test_logger() {
  int i{0};
  LOG_TRACE("%d: Test Log Level %s", i++, "Trace");
  LOG_DEBUG("%d: Test Log Level %s", i++, "Debug");
  LOG_INFO("%d: Test Log Level %s", i++, "Info");
  LOG_WARN("%d: Test Log Level %s", i++, "Warn");
  LOG_ERROR("%d: Test Log Level %s", i++, "Error");
  LOG_FATAL("%d: Test Log Level %s", i++, "Fatal");

  auto message1 = std::stringstream();
  message1 << i << ": Test Log Level with new logger.";

  int a = 12;
  int b = 56;
  auto message2 = std::stringstream();
  message2 << a << " + " << b << " = " << xuzy::add(a, b);

  // Getting the logger instance.
  auto& logger = Logger::get_logger();
  Logger::set_output(&std::cout);

  logger.FATAL(message1.str(), __LINE__, __FILE__);
  logger.ERROR(message1.str(), __LINE__, __FILE__);
  logger.WARN(message1.str(), __LINE__, __FILE__);
  logger.INFO(message2.str());
  logger.DEBUG(message1.str(), __LINE__, __FILE__);
  logger.TRACE(message1.str(), __LINE__, __FILE__);
}

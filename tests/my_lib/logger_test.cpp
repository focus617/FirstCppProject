
#include "logger.h"

#include <iostream>

#include "xuzy_math.h"

using xuzy::Logger;

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
#include "xuzy_math.h"

#include <glog/logging.h>

#include <iostream>

#include "logger.h"

int xuzy::add(int a, int b) {
  LOG(INFO) << xuzy::return_by_macro();
  return a + b;
}

const char* xuzy::return_by_macro() {
#ifdef USE_MYLIB
  return "USE_MYLIB is defined";
#else
  return "USE_MYLIB is not defined";
#endif
}
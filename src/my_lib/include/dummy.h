#pragma once
#include <string>

#include "foo_if.h"

class Dummy {
 private:
  FooIf& m_foo;

 public:
  Dummy(FooIf& f);
  ~Dummy();

  int dummy_int(const std::string& t_str);
  void dummy_str(std::string& t_str);
  void dummy_throw();
  void dummy_callback(std::function<void(void)> t_callback);
};

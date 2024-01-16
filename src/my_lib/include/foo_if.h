#pragma once

#include <functional>
#include <string>

class FooIf {
 public:
  virtual int foo_int(const std::string& t_str) = 0;

  virtual void foo_str(std::string& t_str) = 0;

  virtual void foo_throw() = 0;

  virtual void foo_callback_method(std::function<void(void)>& t_callback) = 0;

  virtual ~FooIf() = default;
};
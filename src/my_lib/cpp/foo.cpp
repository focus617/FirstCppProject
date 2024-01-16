#include "foo.h"

int Foo::foo_int(const std::string& t_str) { return t_str.size(); }

void Foo::foo_str(std::string& t_str) {
  t_str = "ChangedString";
  return;
}

void Foo::foo_throw() {
  // throw runtime error
}

void Foo::foo_callback_method(std::function<void(void)>& t_callback) {
  t_callback();
}
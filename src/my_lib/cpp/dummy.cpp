#include "dummy.h"

Dummy::Dummy(FooIf& f) : m_foo(f) {}

Dummy::~Dummy() {}

int Dummy::dummy_int(const std::string& t_str) { return m_foo.foo_int(t_str); }

void Dummy::dummy_str(std::string& t_str) {
  m_foo.foo_str(t_str);
  return;
}

void Dummy::dummy_throw() { m_foo.foo_throw(); }

void Dummy::dummy_callback(std::function<void(void)> t_callback) {
  m_foo.foo_callback_method(t_callback);
}
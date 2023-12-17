#include "dummy.h"

Dummy::Dummy(FooIf& f):m_Foo(f)
{
}

Dummy::~Dummy()
{
}

int Dummy::dummy_int(const std::string& str)
{
    return m_Foo.foo_int(str);
}

void Dummy::dummy_str(std::string& str)
{
    m_Foo.foo_str(str);
    return;
}
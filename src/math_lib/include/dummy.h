#pragma once
#include <string>

#include "foo_if.h"

class Dummy
{
private:
    FooIf& m_Foo;

public:
    Dummy(FooIf& f);
    ~Dummy();

    int dummy_int(const std::string& str);
    void dummy_str(std::string& str);
};



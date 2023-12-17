#include "foo.h"

int Foo::foo_int(const std::string& str)
{
    return str.size();
}

void Foo::foo_str(std::string& str)
{
    str = "ChangedString";
    return;
}
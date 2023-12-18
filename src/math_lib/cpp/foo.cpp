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

void Foo::foo_throw()
{
    // throw runtime error
}

void Foo::foo_callback_method(std::function<void(void)>& callback)
{
    callback();
}
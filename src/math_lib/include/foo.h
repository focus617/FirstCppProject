#pragma once

#include "foo_if.h"

class Foo: public FooIf
{
    public:
        virtual int foo_int(const std::string& str);

        virtual void foo_str(std::string& str);

        virtual void foo_throw();

        virtual void foo_callback_method(std::function<void(void)>& callback);
};
#pragma once

#include <string>
#include <functional>

class FooIf
{
    public:
        virtual int foo_int(const std::string& str) = 0;

        virtual void foo_str(std::string& str) = 0;

        virtual void foo_throw() = 0;

        virtual void foo_callback_method(std::function<void(void)>& callback) = 0;

};
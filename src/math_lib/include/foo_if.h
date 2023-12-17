#pragma once

#include <string>

class FooIf
{
    public:
        virtual int foo_int(const std::string& str) = 0;

        virtual void foo_str(std::string& str) = 0;
};
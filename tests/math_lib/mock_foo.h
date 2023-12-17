#pragma once
#include <gmock/gmock.h>

#include "foo_if.h"

class MockFoo : public FooIf
{

public:
    MOCK_METHOD(int, foo_int, (const std::string& str), (override));
    MOCK_METHOD(void, foo_str, (std::string& str), (override));
};

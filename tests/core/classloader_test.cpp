#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "classloader/shared_library.h"

#include "dynamicclassregistry.hpp"
#include "shape.hpp"

using namespace dynamicclassloader;

class ClassLoader_Test_Fixture : public testing::Test
{
public:
    ClassLoader_Test_Fixture();
    ~ClassLoader_Test_Fixture();
    void SetUp();
    void TearDown();

    SharedLibrary *library_;

private:
    std::string GetLib()
    {
        std::stringstream ss{};
        ss << RUNTIME_LIB_DIR << "/";
        ss << "libsample_libd.so";
        return ss.str();
    }
};

ClassLoader_Test_Fixture::ClassLoader_Test_Fixture()
{
    library_ = new SharedLibrary(GetLib());
}

ClassLoader_Test_Fixture::~ClassLoader_Test_Fixture()
{
    delete library_;
}

void ClassLoader_Test_Fixture::SetUp()
{
}

void ClassLoader_Test_Fixture::TearDown()
{
}

TEST_F(ClassLoader_Test_Fixture, basic_test)
{
    // It is necessary to destroy the loaded plugins before destroying the class loader.
    {
        auto square =
            library_->LoadClass<Shape, CreateSquareSymbol, DestroySquareSymbol>(
                kSquareCreate, kRectangleCreate, 5);

        std::cout << "Shape: " << square->get_name() << "; Area: "
                  << square->Area() << std::endl;
    }

    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
}
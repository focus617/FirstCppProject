#include <gtest/gtest.h>
#include <iostream>

#include "classloader/shared_library.hpp"

#include "dynamicclassregistry.hpp"
#include "shape.hpp"

using namespace class_loader;


TEST(SharedLibraryTest, basicLoad)
{
    EXPECT_EQ("libtestd.so", SharedLibrary::systemLibraryFormat("test"));
}

class SharedLibrary_Test_Fixture : public testing::Test
{
public:
    SharedLibrary_Test_Fixture();
    ~SharedLibrary_Test_Fixture();
    void SetUp();
    void TearDown();

    static std::string GetLib()
    {
        std::stringstream ss{};
        ss << RUNTIME_LIB_DIR << "/";
        ss << SharedLibrary::systemLibraryFormat("sample_lib");
        return ss.str();
    }

private:
};

SharedLibrary_Test_Fixture::SharedLibrary_Test_Fixture()
{
}

SharedLibrary_Test_Fixture::~SharedLibrary_Test_Fixture()
{
}

void SharedLibrary_Test_Fixture::SetUp()
{
}

void SharedLibrary_Test_Fixture::TearDown()
{
}

TEST_F(SharedLibrary_Test_Fixture, library_loaded_unload_correctly)
{
    SharedLibrary library_;
    ASSERT_FALSE(library_.isLoaded());

    library_.load(GetLib());
    ASSERT_TRUE(library_.isLoaded());

    library_.unload();
    ASSERT_FALSE(library_.isLoaded());

    std::cout << SharedLibrary::systemLibraryFormat("sample_lib") << std::endl;
}

TEST_F(SharedLibrary_Test_Fixture, library_loaded_from_constructor)
{
    SharedLibrary library_(GetLib());
    ASSERT_TRUE(library_.isLoaded());

    library_.unload();
    ASSERT_FALSE(library_.isLoaded());
}

TEST_F(SharedLibrary_Test_Fixture, library_loaded_already_exist_library)
{
    SharedLibrary library_(GetLib());

    ASSERT_THROW(library_.load(GetLib()), xuzy::LibraryAlreadyLoadedException);

    ASSERT_TRUE(library_.isLoaded());

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, library_loaded_nonexist_library)
{
    std::string libraryPath = "NonExistentLibrary";
    SharedLibrary library_;

    ASSERT_THROW(library_.load(libraryPath), xuzy::LibraryLoadException);

    ASSERT_FALSE(library_.isLoaded());
}

TEST_F(SharedLibrary_Test_Fixture, library_get_path_same_as_load)
{
    SharedLibrary library_;
    library_.load(GetLib());

    ASSERT_EQ(GetLib(), library_.getPath());

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, library_get_path_same_as_constructor)
{
    SharedLibrary library_(GetLib());

    ASSERT_EQ(GetLib(), library_.getPath());

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, if_has_noexist_symbol)
{
    std::string symbolName = "NonExistentSymbol";
    SharedLibrary library_(GetLib());

    ASSERT_FALSE(library_.hasSymbol(symbolName));

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, if_has_exist_symbol)
{
    std::string symbolName = "gimmeFive";
    SharedLibrary library_(GetLib());

    ASSERT_TRUE(library_.hasSymbol(symbolName));

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, get_noexist_symbol)
{
    std::string symbolName = "NonExistentSymbol";
    SharedLibrary library_(GetLib());

    ASSERT_THROW(
        library_.getSymbol<char>(symbolName), xuzy::NotFoundException);

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, get_exist_symbol)
{
    std::string symbolName = "gimmeFive";
    SharedLibrary library_(GetLib());

    typedef int (*FunctionPointer)();
    FunctionPointer *p_fun;

    ASSERT_NO_THROW(p_fun = library_.getSymbol<FunctionPointer>(symbolName));
    EXPECT_NE(p_fun, nullptr);

    library_.unload();
}

TEST_F(SharedLibrary_Test_Fixture, dynamically_load_shape_class)
{
    SharedLibrary library_(GetLib());

    // It is necessary to destroy the loaded plugins before destroying the class loader.
    {
        auto square =
            library_.LoadClass<Shape, CreateSquareSymbol, DestroySquareSymbol>(
                kSquareCreate, kRectangleCreate, 5);

        ASSERT_EQ("Square", square->get_name());
        ASSERT_EQ(25, square->Area());
    }

    library_.unload();
}
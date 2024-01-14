#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

#include "classloader/class_loader.hpp"
#include "classloader/shared_library.hpp"
#include "../sample_plugins/base.hpp"

using namespace class_loader;

const std::string LIBRARY_1 = SharedLibrary::systemLibraryFormat("class_loader_TestPlugins1"); // NOLINT
const std::string LIBRARY_2 = SharedLibrary::systemLibraryFormat("class_loader_TestPlugins2"); // NOLINT

TEST(ClassLoaderTest, correct_load_existent_library)
{
    try
    {
        EXPECT_NO_THROW(class_loader::ClassLoader loader(LIBRARY_1, false));
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }
}

TEST(ClassLoaderTest, load_nonexistent_library_throw_LibraryLoadException)
{
    try
    {
        EXPECT_THROW(
            class_loader::ClassLoader loader("libDoesNotExist.so", false),
            xuzy::LibraryLoadException);
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }
}

TEST(ClassLoaderTest, correct_nonlazy_load_unload)
{
    try
    {
        EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));

        class_loader::ClassLoader loader1(LIBRARY_1, false);
        EXPECT_TRUE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));
        EXPECT_TRUE(loader1.isLibraryLoaded());

        EXPECT_NO_THROW(loader1.unloadLibrary());
        EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));
        EXPECT_FALSE(loader1.isLibraryLoaded());
        return;
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }
}

TEST(ClassLoaderTest, correct_lazy_load_unload)
{
    try
    {
        EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));

        class_loader::ClassLoader loader1(LIBRARY_1, true);
        EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));
        EXPECT_FALSE(loader1.isLibraryLoaded());

        {
            std::shared_ptr<Base> obj = loader1.createInstance<Base>("Cat");
            EXPECT_TRUE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));
            EXPECT_TRUE(loader1.isLibraryLoaded());
        }

        // The library will unload automatically when the only plugin object left is destroyed
        EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY_1));
        return;
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }
}

TEST(ClassLoaderTest, correct_load_existent_class)
{
    try
    {
        class_loader::ClassLoader loader(LIBRARY_1, false);
        // See if lazy load works
        EXPECT_NO_THROW(loader.createInstance<Base>("Cat")->saySomething());
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }

    SUCCEED();
}

TEST(ClassLoaderTest, load_nonexistent_class_throw_ClassLoaderException)
{
    try
    {
        class_loader::ClassLoader loader(LIBRARY_1, false);
        // See if lazy load works
        EXPECT_THROW(
            loader.createInstance<Base>("NonExistentClass"), xuzy::CreateClassException);
    }
    catch (xuzy::Exception &e)
    {
        FAIL() << "ClassLoaderException: " << e.what() << "\nMessage: "
               << e.message() << std::endl;
    }
    catch (...)
    {
        FAIL() << "Unhandled exception";
    }

    SUCCEED();
}

TEST(ClassLoaderTest, invalid_base_class)
{
    try
    {
        class_loader::ClassLoader loader1(LIBRARY_1, false);
        if (loader1.isClassAvailable<InvalidBase>("Cat"))
        {
            FAIL() << "Cat should not be available for InvalidBase";
        }
        else if (loader1.isClassAvailable<Base>("Cat"))
        {
            SUCCEED();
            return;
        }
        else
        {
            FAIL() << "Class not available for correct base class.";
        }
    }
    catch (const xuzy::Exception &)
    {
        FAIL() << "Unexpected exception";
    }
    catch (...)
    {
        FAIL() << "Unexpected and unknown exception caught.\n";
    }
}

void wait(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void run(class_loader::ClassLoader *loader)
{
    std::vector<std::string> classes = loader->getAvailableClasses<Base>();
    for (auto &class_ : classes)
    {
        loader->createInstance<Base>(class_)->saySomething();
    }
}

TEST(ClassLoaderTest, thread_safety)
{
    class_loader::ClassLoader loader1(LIBRARY_1);
    ASSERT_TRUE(loader1.isLibraryLoaded());

    // Note: Hard to test thread safety to make sure memory isn't corrupted.
    // The hope is this test is hard enough that once in a while it'll segfault
    // or something if there's some implementation error.
    try
    {
        std::vector<std::thread *> client_threads;

        for (size_t c = 0; c < 1000; c++)
        {
            client_threads.push_back(
                new std::thread(std::bind(&run, &loader1)));
        }

        for (auto &client_thread : client_threads)
        {
            client_thread->join();
        }

        for (auto &client_thread : client_threads)
        {
            delete (client_thread);
        }

        loader1.unloadLibrary();
        ASSERT_FALSE(loader1.isLibraryLoaded());
    }
    catch (const xuzy::Exception &)
    {
        FAIL() << "Unexpected ClassLoaderException.";
    }
    catch (...)
    {
        FAIL() << "Unknown exception.";
    }
}

TEST(ClassLoaderTest, loadRefCountingNonLazy)
{
    try
    {
        class_loader::ClassLoader loader1(LIBRARY_1, false);
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.loadLibrary();
        loader1.loadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_FALSE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_FALSE(loader1.isLibraryLoaded());

        loader1.loadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        return;
    }
    catch (const xuzy::Exception &)
    {
        FAIL() << "Unexpected exception.\n";
    }
    catch (...)
    {
        FAIL() << "Unknown exception caught.\n";
    }

    FAIL() << "Did not throw exception as expected.\n";
}

TEST(ClassLoaderTest, loadRefCountingLazy)
{
    try
    {
        class_loader::ClassLoader loader1(LIBRARY_1, true);
        ASSERT_FALSE(loader1.isLibraryLoaded());

        {
            std::shared_ptr<Base> obj = loader1.createInstance<Base>("Dog");
            ASSERT_TRUE(loader1.isLibraryLoaded());
        }

        ASSERT_FALSE(loader1.isLibraryLoaded());

        loader1.loadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.loadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_FALSE(loader1.isLibraryLoaded());

        loader1.unloadLibrary();
        ASSERT_FALSE(loader1.isLibraryLoaded());

        loader1.loadLibrary();
        ASSERT_TRUE(loader1.isLibraryLoaded());

        return;
    }
    catch (const xuzy::Exception &)
    {
        FAIL() << "Unexpected exception.\n";
    }
    catch (...)
    {
        FAIL() << "Unknown exception caught.\n";
    }

    FAIL() << "Did not throw exception as expected.\n";
}

// void testMultiClassLoader(bool lazy)
// {
//     try
//     {
//         class_loader::MultiLibraryClassLoader loader(lazy);
//         loader.loadLibrary(LIBRARY_1);
//         loader.loadLibrary(LIBRARY_2);
//         for (int i = 0; i < 2; ++i)
//         {
//             loader.createInstance<Base>("Cat")->saySomething();
//             loader.createInstance<Base>("Dog")->saySomething();
//             loader.createInstance<Base>("Robot")->saySomething();
//         }
//     }
//     catch (xuzy::Exception &e)
//     {
//         FAIL() << "ClassLoaderException: " << e.what() << "\n";
//     }

//     SUCCEED();
// }

// TEST(MultiClassLoaderTest, lazyLoad)
// {
//     testMultiClassLoader(true);
// }
// TEST(MultiClassLoaderTest, lazyLoadSecondTime)
// {
//     testMultiClassLoader(true);
// }
// TEST(MultiClassLoaderTest, nonLazyLoad)
// {
//     testMultiClassLoader(false);
// }
// TEST(MultiClassLoaderTest, noWarningOnLazyLoad)
// {
//     try
//     {
//         boost::shared_ptr<Base> cat, dog, rob;
//         {
//             class_loader::MultiLibraryClassLoader loader(true);
//             loader.loadLibrary(LIBRARY_1);
//             loader.loadLibrary(LIBRARY_2);

//             cat = loader.createInstance<Base>("Cat");
//             dog = loader.createInstance<Base>("Dog");
//             rob = loader.createInstance<Base>("Robot");
//         }
//         cat->saySomething();
//         dog->saySomething();
//         rob->saySomething();
//     }
//     catch (xuzy::Exception &e)
//     {
//         FAIL() << "ClassLoaderException: " << e.what() << "\n";
//     }

//     SUCCEED();
// }


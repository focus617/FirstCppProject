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

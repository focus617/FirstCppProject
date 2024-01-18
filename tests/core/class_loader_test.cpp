#include "classloader/class_loader.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

#include "../sample_plugins/base.hpp"
#include "classloader/shared_library.hpp"

using namespace class_loader;

const std::string LIBRARY =
    SharedLibrary::systemLibraryFormat("class_loader_TestPlugins1");  // NOLINT

TEST(ClassLoaderTest, correct_load_existent_library) {
  try {
    EXPECT_NO_THROW(class_loader::ClassLoader loader(LIBRARY, false));
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest,
     nonlazy_load_nonexistent_library_throw_LibraryNotFoundException) {
  try {
    EXPECT_THROW(class_loader::ClassLoader loader("libDoesNotExist.so", false),
                 xuzy::LibraryNotFoundException);
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest,
     lazy_load_nonexistent_library_NOT_throw_LibraryNotFoundException) {
  try {
    EXPECT_NO_THROW(
        class_loader::ClassLoader loader("libDoesNotExist.so", true));
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest,
     lazy_load_nonexistent_class_throw_LibraryNotFoundException) {
  try {
    class_loader::ClassLoader loader("libDoesNotExist.so", true);
    EXPECT_THROW(std::shared_ptr<Base> obj = loader.createInstance<Base>("Cat"),
                 xuzy::LibraryNotFoundException);
  } catch (xuzy::LibraryUnLoadException& e) {
    SUCCEED() << "ClassLoaderException: " << e.what()
              << "\nMessage: " << e.message() << std::endl;
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest, correct_nonlazy_load_unload) {
  try {
    EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));

    class_loader::ClassLoader loader(LIBRARY, false);
    EXPECT_TRUE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));
    EXPECT_TRUE(loader.isLibraryLoaded());

    EXPECT_NO_THROW(loader.unloadLibrary());
    EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));
    EXPECT_FALSE(loader.isLibraryLoaded());
    return;
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest, correct_lazy_load_unload) {
  try {
    EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));

    class_loader::ClassLoader loader(LIBRARY, true);
    EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));
    EXPECT_FALSE(loader.isLibraryLoaded());

    {
      // See if lazy load works
      std::shared_ptr<Base> obj = loader.createInstance<Base>("Cat");
      EXPECT_TRUE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));
      EXPECT_TRUE(loader.isLibraryLoaded());
    }

    // The library will unload automatically when the only plugin object left is
    // destroyed
    EXPECT_FALSE(class_loader::impl::isLibraryLoadedByAnybody(LIBRARY));
    return;
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }
}

TEST(ClassLoaderTest, correct_load_existent_class) {
  try {
    class_loader::ClassLoader loader(LIBRARY, false);

    EXPECT_NO_THROW(loader.createInstance<Base>("Cat")->saySomething());
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }

  SUCCEED();
}

TEST(ClassLoaderTest, correct_load_existent_class_with_unique_ptr) {
  try {
    class_loader::ClassLoader loader(LIBRARY, false);

    EXPECT_NO_THROW(loader.createUniqueInstance<Base>("Cat")->saySomething());
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }

  SUCCEED();
}

TEST(ClassLoaderTest, load_nonexistent_class_throw_ClassLoaderException) {
  try {
    class_loader::ClassLoader loader(LIBRARY, false);
    // See if lazy load works
    EXPECT_THROW(loader.createInstance<Base>("NonExistentClass"),
                 xuzy::CreateClassException);
  } catch (xuzy::Exception& e) {
    FAIL() << "ClassLoaderException: " << e.what()
           << "\nMessage: " << e.message() << std::endl;
  } catch (...) {
    FAIL() << "Unhandled exception";
  }

  SUCCEED();
}

TEST(ClassLoaderTest, invalid_base_class) {
  try {
    class_loader::ClassLoader loader(LIBRARY, false);
    if (loader.isClassAvailable<InvalidBase>("Cat")) {
      FAIL() << "Cat should not be available for InvalidBase";
    } else if (loader.isClassAvailable<Base>("Cat")) {
      SUCCEED();
      return;
    } else {
      FAIL() << "Class not available for correct base class.";
    }
  } catch (const xuzy::Exception&) {
    FAIL() << "Unexpected exception";
  } catch (...) {
    FAIL() << "Unexpected and unknown exception caught.\n";
  }
}

void wait(int seconds) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void run(class_loader::ClassLoader* loader) {
  std::vector<std::string> classes = loader->getAvailableClasses<Base>();
  for (auto& class_ : classes) {
    loader->createInstance<Base>(class_)->saySomething();
  }
}

TEST(ClassLoaderTest, thread_safety) {
  class_loader::ClassLoader loader(LIBRARY);
  ASSERT_TRUE(loader.isLibraryLoaded());

  // Note: Hard to test thread safety to make sure memory isn't corrupted.
  // The hope is this test is hard enough that once in a while it'll segfault
  // or something if there's some implementation error.
  try {
    std::vector<std::thread*> client_threads;

    for (size_t c = 0; c < 1000; c++) {
      client_threads.push_back(new std::thread(std::bind(&run, &loader)));
    }

    for (auto& client_thread : client_threads) {
      client_thread->join();
    }

    for (auto& client_thread : client_threads) {
      delete (client_thread);
    }

    loader.unloadLibrary();
    ASSERT_FALSE(loader.isLibraryLoaded());
  } catch (const xuzy::Exception&) {
    FAIL() << "Unexpected ClassLoaderException.";
  } catch (...) {
    FAIL() << "Unknown exception.";
  }
}

TEST(ClassLoaderTest, loadRefCountingNonLazy) {
  try {
    class_loader::ClassLoader loader(LIBRARY, false);
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.loadLibrary();
    loader.loadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_FALSE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_FALSE(loader.isLibraryLoaded());

    loader.loadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    return;
  } catch (const xuzy::Exception&) {
    FAIL() << "Unexpected exception.\n";
  } catch (...) {
    FAIL() << "Unknown exception caught.\n";
  }

  FAIL() << "Did not throw exception as expected.\n";
}

TEST(ClassLoaderTest, loadRefCountingLazy) {
  try {
    class_loader::ClassLoader loader(LIBRARY, true);
    ASSERT_FALSE(loader.isLibraryLoaded());

    {
      std::shared_ptr<Base> obj = loader.createInstance<Base>("Dog");
      ASSERT_TRUE(loader.isLibraryLoaded());
    }

    ASSERT_FALSE(loader.isLibraryLoaded());

    loader.loadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.loadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_FALSE(loader.isLibraryLoaded());

    loader.unloadLibrary();
    ASSERT_FALSE(loader.isLibraryLoaded());

    loader.loadLibrary();
    ASSERT_TRUE(loader.isLibraryLoaded());

    return;
  } catch (const xuzy::Exception&) {
    FAIL() << "Unexpected exception.\n";
  } catch (...) {
    FAIL() << "Unknown exception caught.\n";
  }

  FAIL() << "Did not throw exception as expected.\n";
}

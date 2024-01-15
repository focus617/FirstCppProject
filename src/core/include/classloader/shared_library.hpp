#pragma once

#include <dlfcn.h>

#include "visibility_control.hpp"
#include "exception.hpp"
#include "shared_library_unix.hpp"

namespace class_loader
{
    /**
     * @brief The SharedLibrary class dynamically loads shared libraries at run-time.
     */
    class XUZY_API SharedLibrary : private SharedLibraryImpl
    {
    public:
        enum Flags
        {
            SHLIB_GLOBAL = 1,
            /// On platforms that use dlopen(), use RTLD_GLOBAL. This is the
            /// default if no flags are given.
            ///
            /// This flag is ignored on platforms that do not use dlopen().

            SHLIB_LOCAL = 2
            /// On platforms that use dlopen(), use RTLD_LOCAL instead of
            /// RTLD_GLOBAL.
            ///
            /// Note that if this flag is specified, RTTI (including
            /// dynamic_cast and throw) will not work for types defined in the
            /// shared library with GCC and possibly other compilers as well.
            /// See http://gcc.gnu.org/faq.html#dso for more information.
            ///
            /// This flag is ignored on platforms that do not use dlopen().
        };

        /**
         * @brief Create a new SharedLibrary object.
         */
        XUZY_API
        SharedLibrary();

        /**
         * @brief Creates a SharedLibrary object and loads a library from the given path.
         */
        XUZY_API
        SharedLibrary(const std::string &path);

        /**
         * @brief Creates a SharedLibrary object and loads a library from the given path, using the given flags.
         * See the Flags enumeration for valid values.
         */
        XUZY_API
        SharedLibrary(const std::string &path, int flags);

        /**
         * @brief Destroys the SharedLibrary. The actual library remains loaded.
         */
        XUZY_API
        virtual ~SharedLibrary();

        /**
         * @brief Loads a shared library from the given path.
         * Throws a LibraryAlreadyLoadedException if a library has already been loaded.
         * Throws a LibraryLoadException if the library cannot be loaded.
         */
        XUZY_API
        void load(const std::string &path);

        /**
         * @brief Loads a shared library from the given path, using the given flags.  See the Flags enumeration for valid values.
         *
         * Throws a LibraryAlreadyLoadedException if a library has already been loaded.
         * Throws a LibraryLoadException if the library cannot be loaded.
         */
        XUZY_API
        void load(const std::string &path, int flags);

        /**
         * @brief Unloads a shared library.
         */
        XUZY_API
        void unload();

        /**
         * @brief
         * @return true if a library has been loaded.
         */
        XUZY_API
        bool isLoaded() const;

        /**
         * @brief
         * @return true if the loaded library contains a symbol with the given name.
         */
        XUZY_API
        bool hasSymbol(const std::string &name);

        /**
         * @brief
         * @return the address of the symbol with the given name.
         * For functions, this is the entry point of the function.
         *
         * Throws a NotFoundException if the symbol does not exist.
         */
        template <typename SymbolType>
        SymbolType *getSymbol(const std::string &name)
        {
            return findSymbolImpl<SymbolType>(name);
        }

        /**
         * @brief
         * @return the path of the library, as specified in a call to load() or the constructor.
         */
        XUZY_API
        const std::string &getPath() const;

        /**
         * @brief
         * @return the platform-specific filename prefix for shared libraries (including the period).
         */
        static std::string systemLibraryPrefix();

        /**
         * @brief
         * @return the platform-specific filename suffix for shared libraries (including the period).
         * In debug mode, the suffix also includes a "d" to specify the debug version of a library (e.g., "d.so", "d.dll") unless the library has been compiled with -DPOCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX.
         */
        static std::string systemLibrarySuffix();

        /**
         * @brief
         * @returns a platform specific version of a basic library name
         *
         * On *nix platforms the library name is prefixed with `lib`.
         * On all platforms the output of class_loader::systemLibrarySuffix() is appended.
         */
        static std::string systemLibraryFormat(const std::string &library_name);

        /**
         * @brief Adds the given path to the list of paths shared libraries are searched in.
         * @return true if the path was set, otherwise false.
         *
         * Currently only supported on Windows, where it calls SetDllDirectory(). On all other platforms, does not do anything and returns false.
         */
        static bool setSearchPath(const std::string &path);

        // Temporary put here
        /**
         * @brief Load a class
         * @param constructor_name The name of the create function symbol
         * @param destructor_name The name of the destroy function smybol
         * @param args Arguments to pass to the actual class construction (arbitrary length)
         * @tparam ClassType The interface type of the class to create
         * @tparam Constructor The type of the create function symbol
         * @tparam Destructor The type of the destroy function symbol
         * @tparam ...ArgTypes The types of the arguments
         * @return A shared pointer to the requested class
         */
        template <typename ClassType,
                  typename Constructor = ClassType *(),
                  typename Destructor = void(ClassType *),
                  typename... ArgTypes>
        std::shared_ptr<ClassType> LoadClass(
            const std::string &constructor_name,
            const std::string &destructor_name,
            ArgTypes... args)
        {
            auto *create = getSymbol<Constructor>(constructor_name);
            auto *destroy = getSymbol<Destructor>(destructor_name);
            return std::shared_ptr<ClassType>(create(args...), destroy);
        }

    private:
        SharedLibrary(const SharedLibrary &);
        SharedLibrary &operator=(const SharedLibrary &);
    };

} // namespace class_loader
cmake_minimum_required(VERSION 3.16.0)

project(my_first_proj VERSION 0.1.0 LANGUAGES CXX)

# set compile feature
set(CMAKE_CXX_STANDARD 20)                  # Default C++ standard for targets
set(CMAKE_CXX_STANDARD_REQUIRED True)       # Make C++20 a hard requirement
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++20 -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++20")
set(EXTRA_COMPILE_OPTIONS "-Wextra -Wshadow -pedantic")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_DEBUG_POSTFIX d)
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")

### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# enable vcpkg
set(CMAKE_TOOLCHAIN_FILE "${PROJECT_SOURCE_DIR}/extern/vcpkg/scripts/buildsystems/vcpkg.cmake")

# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(cmake/MultiThreadBuild.cmake)

# update all submodule
# include(cmake/CheckGitSubmodule.cmake)
# build all submodule
add_subdirectory(extern)

# Indicate the code style format as an option
option(FORMAT_CODE "Clang-format code" ON)
if(FORMAT_CODE)
    message(STATUS "Clang-format Code...")
    include(cmake/clang-format.cmake)
endif()

# Indicate the code style format as an option
option(STATIC_ANALYSIZE_CODE "Static Analysize code" ON)
if (STATIC_ANALYSIZE_CODE)
    message(STATUS "Cpp-Check Code...")
    include(cmake/cppcheck.cmake)
endif()

# build my module
add_subdirectory(src)

# build my test
include(CTest)
if(BUILD_TESTING)
    message(STATUS "Building Testing...")
    add_subdirectory(tests)
endif()

# Indicate the documentation build as an option
option(BUILD_DOC "Build documentation" ON)
if(BUILD_DOC)
    message(STATUS "Building Document...")
    include(cmake/Doxygen.cmake)
endif()

# Set installation directory    
set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install")
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

include(InstallRequiredSystemLibraries)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/License.txt")
include(CPack)


configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
)

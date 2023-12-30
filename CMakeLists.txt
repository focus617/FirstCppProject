cmake_minimum_required(VERSION 3.16.0)

project(my_first_proj VERSION 0.1.0 LANGUAGES C CXX)

# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# set compile feature
set(CMAKE_CXX_STANDARD 20)                  # Default C++ standard for targets
set(CMAKE_CXX_STANDARD_REQUIRED True)       # Make C++20 a hard requirement
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++20")
set(EXTRA_COMPILE_OPTIONS "-Wextra -Wshadow -Wnon-virtual-dtor -pedantic")

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")   # Set executable output directory

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
)

set(CMAKE_DEBUG_POSTFIX d)

# update all submodule
include(cmake/gitsubmodule.cmake)

add_subdirectory(src)

add_subdirectory(extern)

include(CTest)
if (BUILD_TESTING)
    message(STATUS "Building Testing...")
    add_subdirectory(tests)
endif()


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

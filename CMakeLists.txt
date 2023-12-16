cmake_minimum_required(VERSION 3.16.0)

project(my_first_prog VERSION 0.1.0 LANGUAGES C CXX)

# enable cache, extra warnings, docs generation, etc
list(APPEND 
    CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
set(CXX_STANDARD_REQUIRED ON)      # Make C++20 a hard requirement
set(CMAKE_CXX_STANDARD 20)         # Default C++ standard for targets

add_subdirectory(src)

option(BUILD_TEST "Build Test" ON)
if (BUILD_TEST)
    message(STATUS "Building Testing...")
    add_subdirectory(tests)
endif()


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

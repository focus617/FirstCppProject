cmake_minimum_required(VERSION 3.16.0)

project(my_first_prog VERSION 0.1.0 LANGUAGES C CXX)

# enable cache, extra warnings, docs generation, etc
list(APPEND 
    CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)


### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

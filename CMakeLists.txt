cmake_minimum_required(VERSION 3.6.0)

project(my_first_prog VERSION 0.1.0 LANGUAGES C CXX)

# enable cache, extra warnings, docs generation, etc
list(APPEND 
    CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

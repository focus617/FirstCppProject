cmake_minimum_required(VERSION 3.6.0)

project(my_first_prog VERSION 0.1.0 LANGUAGES C CXX)

# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

option(BUILD_MAIN "Build Main Program" ON)
option(BUILD_MATH_LIB "Build Math Lib" ON)

if (BUILD_MATH_LIB)
    message(STATUS "Building My Math Library...")
    include_directories ("${PROJECT_SOURCE_DIR}/math_lib/include")
    add_subdirectory(math_lib)
    set (EXTRA_LIBS ${EXTRA_LIBS} xuzymath)
endif()

if (BUILD_MAIN)
    message(STATUS "Building Main Program...")
    include_directories ("${PROJECT_SOURCE_DIR}/main_prog/include")
    add_subdirectory(main_prog)
endif()


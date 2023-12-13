# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH "${SOURCE_DIR}/cmake")


message(STATUS "Building Core Library...")
include_directories ("${SOURCE_DIR}/core/include")
add_subdirectory(core)
set (EXTRA_LIBS ${EXTRA_LIBS} core)


option(BUILD_MAIN "Build Main Program" ON)
option(BUILD_MATH_LIB "Build Math Lib" ON)

if (BUILD_MATH_LIB)
    message(STATUS "Building My Math Library...")
    include_directories ("${SOURCE_DIR}/math_lib/include")
    add_subdirectory(math_lib)
    set (EXTRA_LIBS ${EXTRA_LIBS} xuzymath)
endif()

if (BUILD_MAIN)
    message(STATUS "Building Main Program...")
    include_directories ("${SOURCE_DIR}/main_prog/include")
    add_subdirectory(main_prog)
endif()


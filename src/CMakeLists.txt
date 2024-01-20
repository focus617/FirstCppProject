# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH "${SOURCE_DIR}/cmake")

message(STATUS "Building Core Library...")
add_subdirectory(core)

option(BUILD_RESTFUL_LIB "Build Restful Library" ON)
option(BUILD_MY_LIB "Build My Library" ON)
option(BUILD_MAIN "Build Main Program" ON)

if (BUILD_RESTFUL_LIB)
    message(STATUS "Building Restful Library...")
    add_subdirectory(restful)
endif()

if (BUILD_MY_LIB)
    message(STATUS "Building My Library...")
    add_subdirectory(my_lib)
    list(APPEND EXTRA_LIBS my_lib)
endif()

if (BUILD_MAIN)
    message(STATUS "Building Main Program...")
    add_subdirectory(main_prog)
endif()


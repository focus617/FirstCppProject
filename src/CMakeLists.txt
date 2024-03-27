# enable cache, extra warnings, docs generation, etc
list(APPEND CMAKE_MODULE_PATH "${SOURCE_DIR}/cmake")

message(STATUS "Building Core Library...")
add_subdirectory(core)

option(BUILD_RESTFUL "Build Restful App" ON)
option(BUILD_WINDOWS "Build Windows App" ON)
option(BUILD_MnistML "Build Mnist ML" ON)

if (BUILD_RESTFUL)
    message(STATUS "Building Restful App...")
    add_subdirectory(restful)
    add_subdirectory(main_restful)
endif()

if (BUILD_WINDOWS)
    message(STATUS "Building Windows App...")
    add_subdirectory(my_lib)
    list(APPEND EXTRA_LIBS my_lib)
    add_subdirectory(main_windows)
endif()

if (BUILD_MnistML)
    message(STATUS "Building Mnist ML...")
    add_subdirectory(mnist_ml)
endif()



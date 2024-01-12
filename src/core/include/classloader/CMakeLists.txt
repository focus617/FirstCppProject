message(STATUS "Building DynamicClassLoader Library...")

add_library(DynamicClassLoader INTERFACE)

target_include_directories(DynamicClassLoader INTERFACE  ".")
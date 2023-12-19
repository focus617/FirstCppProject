set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

option(ABSL_PROPAGATE_CXX_STD "" ON)
add_subdirectory(abseil)
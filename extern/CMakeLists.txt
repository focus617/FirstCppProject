set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

option(ABSL_PROPAGATE_CXX_STD "" ON)
# option(ABSL_BUILD_TESTING "" ON)
# option(ABSL_USE_GOOGLETEST_HEAD "" ON)
add_subdirectory(abseil)
message(STATUS "Build GoogleTest...")
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(googletest)

message(STATUS "Build Glog...")
set(GTest_DIR "${PROJECT_SOURCE_DIR}/install/lib/cmake/GTest") 
add_subdirectory(glog)

message(STATUS "Build Cpp-httplib...")
add_subdirectory(cpp-httplib)

message(STATUS "Build Abseil...")
# add_subdirectory(abseil)

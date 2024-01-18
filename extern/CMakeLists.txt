message(STATUS "Build GoogleTest...")
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(googletest)

message(STATUS "Build Glog...")
set(GTest_DIR "${PROJECT_SOURCE_DIR}/install/lib/cmake/GTest") 
add_subdirectory(glog)

# message(STATUS "Build Abseil...")
# add_subdirectory(abseil)

message(STATUS "Build Cpp-httplib...")
add_subdirectory(cpp-httplib)

message(STATUS "Build Nlohmann-json...")
# If you only include this third party in PRIVATE source files, you do not need to install it, when your main project gets installed.
# normally should be OFF for development
set(JSON_Install OFF CACHE INTERNAL "")
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann-json)
install(TARGETS nlohmann_json EXPORT nlohmann_json DESTINATION include)
install(EXPORT nlohmann_json DESTINATION share/cmake/nlohmann_json)

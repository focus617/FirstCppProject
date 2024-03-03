cmake_minimum_required(VERSION 3.21)

project(glad_project)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
find_package(OpenGL REQUIRED)

add_library(glad SHARED src/glad.c)

target_include_directories(glad
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<INSTALL_INTERFACE:include/glad>
)

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

# 安装库
install(
  TARGETS glad
  EXPORT gladTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  BUNDLE DESTINATION bin COMPONENT Runtime)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "gladConfigVersion.cmake"
  VERSION "2.0.0"
  COMPATIBILITY AnyNewerVersion)

install(
  EXPORT gladTargets
  FILE gladTargets.cmake
  DESTINATION share/cmake/glad)

install(FILES
  "gladConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/gladConfigVersion.cmake" 
  DESTINATION share/cmake/glad
 )

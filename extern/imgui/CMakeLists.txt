cmake_minimum_required(VERSION 3.21)

project(imgui_project)

set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(GLFW_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/glfw/include
  CACHE PATH "GLFW include directory"
)

add_library(imgui STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imstb_rectpack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imstb_textedit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imstb_truetype.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_draw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_internal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_tables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
 )

target_include_directories(imgui
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/backends>
  PUBLIC $<INSTALL_INTERFACE:include/imgui>
  PRIVATE ${GLFW_INCLUDE_DIR}
)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)
target_compile_options(imgui PRIVATE -fPIC)

# 安装库
install(
  TARGETS imgui
  EXPORT imguiTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include/imgui COMPONENT Development
  BUNDLE DESTINATION bin COMPONENT Runtime)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "imguiConfigVersion.cmake"
  VERSION "2.0.0"
  COMPATIBILITY AnyNewerVersion)

install(
  EXPORT imguiTargets
  FILE imguiTargets.cmake
  DESTINATION share/cmake/imgui)

install(FILES 
  "imguiConfig.cmake" 
  "${CMAKE_CURRENT_BINARY_DIR}/imguiConfigVersion.cmake" 
  DESTINATION share/cmake/imgui
)

install(FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui.h
  ${CMAKE_CURRENT_SOURCE_DIR}/imconfig.h 
  DESTINATION include/imgui
)

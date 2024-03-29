set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
# Import extern Libraries
find_package(glog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)

# Import nlohmann_json
find_package(nlohmann_json 3.11.3 REQUIRED)
if(nlohmann_json_FOUND)
  message(STATUS "nlohmann_json 3.11.3 Found")
endif()

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(app)
add_subdirectory(classloader)
add_subdirectory(tools)
add_subdirectory(window)
add_subdirectory(ui)
add_subdirectory(editor)
add_subdirectory(renderer)
add_subdirectory(app/include/resource)

# 生成动态链接库
add_library(core SHARED ${_SOURCES})

target_compile_features(core PUBLIC cxx_std_20)
set_target_properties(core PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(core PRIVATE XUZY_DLL XUZY_DLL_EXPORTS)
target_compile_definitions(core PRIVATE NDEBUG _DEBUG)

# 添加编译选项
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(MY_DEBUG_OPTIONS -Wall -Werror -Wextra -Wshadow -pedantic -Wno-unused-parameter -Wmain -fstack-usage -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(MY_DEBUG_OPTIONS -Wall -Wconversion -Wdouble-promotion -Weffc++ -Wextra  -Wmain -Wshadow -pedantic -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(MY_DEBUG_OPTIONS /Wall /RTC)
  set(MY_RELEASE_OPTIONS /w3 /O2)
endif()

target_compile_options(core PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(core PRIVATE 
                                   "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")

set(NLOHMANN_JSON_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include
  CACHE PATH "nlohmann/json include directory"
)

set(GLFW_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/glfw/include
  CACHE PATH "GLFW include directory"
)

set(GLAD_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/glad/include
  CACHE PATH "GLAD include directory"
)

set(GLM_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/glm
  CACHE PATH "GLM include directory"
)

# 指定头文件路径
target_include_directories(core
  PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/classloader/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/editor/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/renderer/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/tools/stb_image>
    $<INSTALL_INTERFACE:include/core>
  PRIVATE
    ${PROJECT_BINARY_DIR}/src
    ${NLOHMANN_JSON_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# Precompiled headers
target_precompile_headers(core 
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/app/include/pch.h"
)

# 添加链接库
target_link_libraries(core
  PUBLIC glog::glog
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC resource
  PUBLIC glfw
  PUBLIC glad
  PRIVATE glm::glm
  PRIVATE ${GL_LIBRARY}  #opengl lib
)

# 安装库
install(TARGETS core
  EXPORT coreTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include/core COMPONENT Development
  BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "coreConfigVersion.cmake"
  VERSION ${my_first_proj_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(EXPORT coreTargets
  FILE coreTargets.cmake
  NAMESPACE xuzy::
  DESTINATION share/cmake/core
)
install(FILES
    "coreConfig.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/coreConfigVersion.cmake" 
    DESTINATION share/cmake/core
)
# 安装字体
install(DIRECTORY resources/fonts/ DESTINATION assets/fonts)

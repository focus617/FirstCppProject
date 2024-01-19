# Import nlohmann_json
set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
find_package(nlohmann_json 3.11.3 REQUIRED)
if(nlohmann_json_FOUND)
  message(STATUS "nlohmann_json 3.11.3 Found")
endif()

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)
add_subdirectory(include/resource)

# 生成静态链接库
add_library(core SHARED ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_COMPILE_OPTIONS}")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

target_compile_definitions(core PRIVATE XUZY_DLL XUZY_DLL_EXPORTS _DEBUG)

set(NLOHMANN_JSON_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include
  CACHE PATH "nlohmann/json include directory"
)

# 指定头文件路径
target_include_directories(core
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PUBLIC $<INSTALL_INTERFACE:include/core>
  PRIVATE ${PROJECT_BINARY_DIR}/src
  PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR}
)

# 添加链接库
target_link_libraries(core
  PUBLIC glog::glog
  PUBLIC nlohmann_json::nlohmann_json
  PUBLIC resource
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

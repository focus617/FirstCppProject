set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
# Import extern Libraries
find_package (absl REQUIRED)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(WindowApp ${_SOURCES})

# 给Debug版本的应用添加后缀
set_target_properties(WindowApp PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_compile_features(WindowApp PUBLIC cxx_std_20)
set_target_properties(WindowApp PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(WindowApp PRIVATE XUZY_DLL )
target_compile_definitions(WindowApp PRIVATE NDEBUG _DEBUG)

# 添加编译选项
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(MY_DEBUG_OPTIONS -Wall -Werror -Wextra -Wshadow -pedantic -Wfloat-equal -Wno-unused-parameter -Wnon-virtual-dtor -Wmain -fstack-usage -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(MY_DEBUG_OPTIONS -Wall -Wconversion -Wdouble-promotion -Weffc++ -Wextra -Wfloat-equal -Wmain -Wshadow -pedantic -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(MY_DEBUG_OPTIONS /Wall /RTC)
  set(MY_RELEASE_OPTIONS /w3 /O2)
endif()

target_compile_options(WindowApp PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(WindowApp PRIVATE 
                                   "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")


set(NLOHMANN_JSON_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include
  CACHE PATH "nlohmann/json include directory"
)

# 指定头文件路径
target_include_directories(WindowApp
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include/xuzy>
    PRIVATE
      ${PROJECT_SOURCE_DIR}/src
      ${PROJECT_BINARY_DIR}/src
      ${NLOHMANN_JSON_INCLUDE_DIR}
)

# 添加链接库
target_link_libraries(WindowApp PUBLIC
     core
     absl::flags
     absl::flags_parse
     absl::flags_usage
     glog::glog
     nlohmann_json::nlohmann_json
     ${EXTRA_LIBS}
)

install (TARGETS WindowApp DESTINATION bin)

# 安装配置文件
install(FILES
    Windows-App.json
    DESTINATION bin
)

install(DIRECTORY assets/textures/ DESTINATION assets/textures)

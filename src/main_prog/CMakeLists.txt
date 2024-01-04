# Import Abseil Library
set(absl_DIR "${PROJECT_SOURCE_DIR}/install/lib/cmake/absl") 
find_package (absl REQUIRED)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(my_app ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wshadow -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wextra -Wshadow -Wno-unused-parameter")

# 给Debug版本的应用添加后缀
set_target_properties(my_app PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# 指定头文件路径
target_include_directories(my_app
     PUBLIC ${EXTRA_INCLUDES}
     PRIVATE ${PROJECT_BINARY_DIR}/src
     PRIVATE ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include/nlohmann
)

# 添加链接库
target_link_libraries(my_app PUBLIC 
     absl::flags
     absl::flags_parse
     absl::flags_usage
     glog::glog
     nlohmann_json::nlohmann_json
     ${EXTRA_LIBS}
)

install (TARGETS my_app DESTINATION bin)

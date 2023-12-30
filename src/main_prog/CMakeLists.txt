
# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(my_app ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wextra -Wshadow")

set_target_properties(my_app PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_property(TARGET my_app PROPERTY VERSION "1.0.0")
set_property(TARGET my_app PROPERTY SOVERSION "1")

# 添加链接库
target_link_libraries(my_app PRIVATE
     ${EXTRA_LIBS}
     absl::flags
     absl::flags_parse
     absl::flags_usage
)

target_include_directories(my_app 
     PRIVATE ${PROJECT_BINARY_DIR}/src
)



# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(my_app ${_SOURCES})

# 添加链接库
target_link_libraries(my_app PUBLIC
     ${EXTRA_LIBS}
     absl::flags
     absl::flags_parse
     absl::flags_usage
)
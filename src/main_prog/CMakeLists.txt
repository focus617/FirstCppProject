
# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(my_app ${_SOURCES})

# 指定编译语言标准
target_compile_features(my_app PRIVATE cxx_std_20)

# 指定头文件路径
target_include_directories(my_app PRIVATE include)

# 添加链接库
target_link_libraries(my_app PRIVATE ${EXTRA_LIBS})

# 指定链接库路径
target_link_directories(my_app PRIVATE ${PROJECT_BINARY_DIR}/math_lib/)



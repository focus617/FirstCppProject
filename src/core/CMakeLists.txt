
# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(include)
add_subdirectory(cpp)

# 生成静态链接库
add_library(core ${_SOURCES})

# 指定头文件路径
target_include_directories(core PRIVATE include)

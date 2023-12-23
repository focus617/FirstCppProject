
# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成静态链接库
add_library(my_lib STATIC ${_SOURCES})

# 指定编译语言标准
target_compile_features(my_lib PRIVATE cxx_std_20)

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${EXTRA_COMPILE_OPTIONS}")

# 指定头文件路径
target_include_directories(my_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# 添加链接库
target_link_libraries(my_lib PRIVATE core)

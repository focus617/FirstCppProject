set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
find_package (core REQUIRED)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成静态链接库
add_library(my_lib STATIC ${_SOURCES})

# 指定编译语言标准
target_compile_features(my_lib PRIVATE cxx_std_20)
set_target_properties(restful PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(my_lib PRIVATE "USE_MYLIB")

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

target_compile_options(my_lib PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(my_lib PRIVATE 
                                   "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")


# 指定头文件路径
target_include_directories(my_lib 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE ${PROJECT_BINARY_DIR}/src
)

# 添加链接库
target_link_libraries(my_lib PRIVATE core)
# target_link_libraries(my_lib PRIVATE xuzy::core)

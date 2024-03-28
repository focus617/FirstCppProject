set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
# Import extern Libraries
find_package (glog REQUIRED)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(data)
add_subdirectory(knn)
add_subdirectory(neural_network)

# 生成可执行程序
add_executable(MnistMl ${_SOURCES})

# 给Debug版本的应用添加后缀
set_target_properties(MnistMl PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_compile_features(MnistMl PUBLIC cxx_std_20)
set_target_properties(MnistMl PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(MnistMl PRIVATE XUZY_DLL EUCLIDEAN NEURAL_NETWORK)
target_compile_definitions(MnistMl PRIVATE NDEBUG _DEBUG)

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

target_compile_options(MnistMl PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(MnistMl PRIVATE 
                                   "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")


# 指定头文件路径
target_include_directories(MnistMl
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/data/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/knn/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/neural_network/include>
      $<INSTALL_INTERFACE:include/xuzy>
)

# 添加链接库
target_link_libraries(MnistMl PUBLIC
    glog::glog
    ${EXTRA_LIBS}
)

install (TARGETS MnistMl DESTINATION bin)

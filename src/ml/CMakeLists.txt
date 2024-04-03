# Import extern Libraries
find_package (glog REQUIRED)

set(PYTHON_EXECUTABLE "/usr/bin/python3")
set(Torch_DIR /usr/local/libtorch/share/cmake/Torch)

find_package (Torch REQUIRED PATHS "/usr/local/libtorch")
if(NOT Torch_FOUND)
    message(FATAL_ERROR "Pytorch Not Found!")
endif(NOT Torch_FOUND)

message(STATUS "Find LibTorch VERSION: ${Torch_VERSION}")
message(STATUS "LibTorch status :")
message(STATUS "    libraries: ${TORCH_LIBRARIES}")
message(STATUS "    Torch Flags: ${TORCH_CXX_FLAGS}")

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -pthread")

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(etl)
add_subdirectory(perceptron)
add_subdirectory(knn)
add_subdirectory(neural_network)

# 生成可执行程序
add_executable(Ml ${_SOURCES})

# 给Debug版本的应用添加后缀
set_target_properties(Ml PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

set_target_properties(Ml PROPERTIES CXX_STANDARD 17)
set_target_properties(Ml PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(Ml PRIVATE XUZY_DLL EUCLIDEAN)
target_compile_definitions(Ml PRIVATE NDEBUG _DEBUG)

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

target_compile_options(Ml PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(Ml PRIVATE "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")


# 指定头文件路径
target_include_directories(Ml
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/etl/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/perceptron/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/knn/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/neural_network/include>
      $<BUILD_INTERFACE:${TORCH_INCLUDE_DIRS}
      $<INSTALL_INTERFACE:include/xuzy>
)

# Precompiled headers
target_precompile_headers(core 
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
)

# 添加链接库
target_link_libraries(Ml PUBLIC
    glog::glog
    ${TORCH_LIBRARIES}
    ${EXTRA_LIBS}
)

install (TARGETS Ml DESTINATION bin)

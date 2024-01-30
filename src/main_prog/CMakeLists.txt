set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
# Import Abseil Library
find_package (absl REQUIRED)
# find_package (core REQUIRED)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)

# 生成可执行程序
add_executable(my_app ${_SOURCES})

target_compile_features(my_app PUBLIC cxx_std_20)
set_target_properties(my_app PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(my_app PRIVATE XUZY_DLL _DEBUG)

# 给Debug版本的应用添加后缀
set_target_properties(my_app PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

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

target_compile_options(my_app PRIVATE "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(my_app PRIVATE 
                                   "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")


set(NLOHMANN_JSON_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include
  CACHE PATH "nlohmann/json include directory"
)

# 指定头文件路径
target_include_directories(my_app
     PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
     PUBLIC $<INSTALL_INTERFACE:include/xuzy>
     PRIVATE ${PROJECT_SOURCE_DIR}/src
     PRIVATE ${PROJECT_BINARY_DIR}/src
     PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR}
)

# 添加链接库
target_link_libraries(my_app PUBLIC
     # xuzy::core
     core
     restful
     absl::flags
     absl::flags_parse
     absl::flags_usage
     glog::glog
     nlohmann_json::nlohmann_json
     ${EXTRA_LIBS}
)

install (TARGETS my_app DESTINATION bin)

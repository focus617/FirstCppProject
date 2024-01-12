# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(cpp)
add_subdirectory(./include/classloader)

# 生成静态链接库
add_library(core SHARED ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${EXTRA_COMPILE_OPTIONS}")

target_compile_definitions(core PUBLIC
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# 指定头文件路径
target_include_directories(core 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE ${PROJECT_BINARY_DIR}/src
    PRIVATE ${PROJECT_SOURCE_DIR}/extern/cpp-httplib
    PRIVATE ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include/nlohmann
)

# 添加链接库
target_link_libraries(core PUBLIC
    glog::glog
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>
    nlohmann_json::nlohmann_json
)

set_target_properties(core PROPERTIES
    PUBLIC_HEADER include/logger.h
)

install(TARGETS core
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/xuzy
)    

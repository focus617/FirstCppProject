set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/install")
find_package (core REQUIRED)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
    message(STATUS "Set HTTPLIB_IS_USING_OPENSSL to ${HTTPLIB_IS_USING_OPENSSL}")
endif()

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _SOURCES)

# 添加子目录
add_subdirectory(api)
add_subdirectory(http)

# 生成静态链接库
add_library(restful SHARED ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS} -Wnon-virtual-dtor")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${EXTRA_COMPILE_OPTIONS} -Wnon-virtual-dtor")

target_compile_definitions(restful 
    PUBLIC $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
    PRIVATE XUZY_DLL XUZY_DLL_EXPORTS
)

# 指定头文件路径
target_include_directories(restful
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/api"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/http"
    PRIVATE ${PROJECT_SOURCE_DIR}/extern/cpp-httplib
    PRIVATE ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include/nlohmann
)

# 添加链接库
target_link_libraries(restful PUBLIC
    # xuzy::core
    core
    glog::glog
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>
)

install(TARGETS restful
    LIBRARY DESTINATION lib
)

install(FILES
         host/cert.pem
         host/key.pem
        DESTINATION bin
)

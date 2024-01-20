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

# 生成动态链接库
add_library(restful SHARED ${_SOURCES})

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS} -Wnon-virtual-dtor")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${EXTRA_COMPILE_OPTIONS} -Wnon-virtual-dtor")

target_compile_definitions(restful 
    PUBLIC $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
    PRIVATE XUZY_DLL XUZY_DLL_EXPORTS  _DEBUG
)

set(NLOHMANN_JSON_INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include
  CACHE PATH "nlohmann/json include directory"
)

# 指定头文件路径
target_include_directories(restful
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PUBLIC $<INSTALL_INTERFACE:include/restful>
    PRIVATE ${PROJECT_SOURCE_DIR}/extern/cpp-httplib
    PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR}
)

# 添加链接库
target_link_libraries(restful PUBLIC
    # xuzy::core
    core
    glog::glog
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>
)

# 为测试安装配置文件
install(FILES
    Restful-Server.json
    DESTINATION ${CMAKE_BINARY_DIR}/bin
)

# 安装库
install(TARGETS restful
  EXPORT restfulTargets
  LIBRARY DESTINATION lib COMPONENT Runtime
  ARCHIVE DESTINATION lib COMPONENT Development
  RUNTIME DESTINATION bin COMPONENT Runtime
  PUBLIC_HEADER DESTINATION include/restful COMPONENT Development
  BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "restfulConfigVersion.cmake"
  VERSION ${my_first_proj_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(EXPORT restfulTargets
  FILE restfulTargets.cmake
  NAMESPACE xuzy::
  DESTINATION share/cmake/restful
)
install(FILES
    "restfulConfig.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/restfulConfigVersion.cmake" 
    DESTINATION share/cmake/restful
)

# Certification
if(HTTPLIB_IS_USING_OPENSSL)
    include(${PROJECT_SOURCE_DIR}/cmake/OpenSSL.cmake)
endif()

install(FILES
    ${CMAKE_BINARY_DIR}/bin/cert.pem
    ${CMAKE_BINARY_DIR}/bin/key.pem
    Restful-Server.json
    DESTINATION bin
)
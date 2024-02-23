message(STATUS "Building Resource Library...")

add_library(resource INTERFACE)

target_include_directories(resource INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/resource>
)

# Install
install(TARGETS resource
    EXPORT resourceTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include/resource COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/src/core/resourceConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/resourceConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/src/core/resourceConfig.cmake"
    INSTALL_DESTINATION share/cmake/resource
)

install(EXPORT resourceTargets DESTINATION share/cmake/resource)
install(FILES "${PROJECT_BINARY_DIR}/src/core/resourceConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/src/core/resourceConfig.cmake"
    DESTINATION share/cmake/resource
)

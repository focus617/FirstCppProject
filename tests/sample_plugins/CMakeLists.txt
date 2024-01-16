message(STATUS "Building Library class_loader_TestPlugins...")

add_library(class_loader_TestPlugins1 SHARED plugins1.cpp)

target_link_libraries(class_loader_TestPlugins1 core)

target_compile_definitions(class_loader_TestPlugins1 
   PRIVATE XUZY_DLL XUZY_DLL_EXPORTS
)

install(TARGETS class_loader_TestPlugins1
   LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/bin"
)

add_library(class_loader_TestPlugins2 SHARED plugins2.cpp)

target_link_libraries(class_loader_TestPlugins2 core)

target_compile_definitions(class_loader_TestPlugins2 
   PRIVATE XUZY_DLL XUZY_DLL_EXPORTS
)

install(TARGETS class_loader_TestPlugins2
   LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/bin"
)

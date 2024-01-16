message(STATUS "Building Library sample_lib...")

add_library(sample_lib SHARED
   square.cpp rectangle.cpp dynamicclassregistry.cpp symbol_test.cpp)

target_compile_definitions(sample_lib 
   PRIVATE XUZY_DLL XUZY_DLL_EXPORTS
)
   
set_target_properties(sample_lib PROPERTIES
   PUBLIC_HEADER dynamicclassregistry.hpp
   PUBLIC_HEADER square.hpp
)

# 指定头文件路径
target_include_directories(sample_lib 
    PRIVATE ${PROJECT_SOURCE_DIR}/src/core/include
)

install(TARGETS sample_lib
   LIBRARY DESTINATION "${CMAKE_BINARY_DIR}/bin"
   PUBLIC_HEADER DESTINATION "${CMAKE_BINARY_DIR}/bin"
)

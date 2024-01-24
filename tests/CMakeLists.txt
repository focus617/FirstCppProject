option(FETCH_GOOGLE_TEST "Fetch GoogleTest" OFF)

if(FETCH_GOOGLE_TEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  #set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  FetchContent_GetProperties(googleTest)

  if (NOT googleTest_POPULATED)
    message(STATUS "Fetching Google Test...")
    # FetchContent_Populate(googleTest)
    # add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
    FetchContent_MakeAvailable(googletest)
  endif()
endif()

### Check google test exist ###
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/googletest/CMakeLists.txt")
  message(FATAL_ERROR "The googletest submodule was not downloaded correctly. GIT_SUBMODULE maybe turned off or failed. Please check!")
endif()

enable_testing()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _TEST_SOURCES)

option(TEST_CORE_LIB "Test Core Library" ON)
option(TEST_RESTFUL_LIB "Test Restful Library" ON)
option(TEST_MY_LIB "Test My Library" ON)

if (TEST_CORE_LIB)
  message(STATUS "Building test for Core Library...")
  add_subdirectory(core)
  add_subdirectory(sample_lib)
  add_subdirectory(sample_plugins)
  list(APPEND TEST_EXTRA_LIBS core)
  list(APPEND TEST_EXTRA_INCLUDES "${SOURCE_DIR}/core/include")
  list(APPEND TEST_EXTRA_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/sample_lib")
endif()

if (TEST_RESTFUL_LIB)
  message(STATUS "Building test for Restful Library...")
  
  find_package(OpenSSL REQUIRED)
  if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
    message(STATUS "Set HTTPLIB_IS_USING_OPENSSL to ${HTTPLIB_IS_USING_OPENSSL}")
  endif()

  add_subdirectory(restful)
  list(APPEND TEST_EXTRA_LIBS restful)
  list(APPEND TEST_EXTRA_INCLUDES 
      "${SOURCE_DIR}/restful/api"
      "${SOURCE_DIR}/restful/http"
  )
  target_compile_definitions(restful PUBLIC
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
  )
endif()

if (TEST_MY_LIB)
  message(STATUS "Building test for My Library...")
  add_subdirectory(my_lib)
  list(APPEND TEST_EXTRA_LIBS my_lib)
  list(APPEND TEST_EXTRA_INCLUDES "${SOURCE_DIR}/my_lib/include")
endif()

# 生成可执行程序
add_executable(testRunner ${_TEST_SOURCES})

# 指定编译语言标准
target_compile_features(testRunner PRIVATE cxx_std_20)
set_target_properties(testRunner PROPERTIES CXX_EXTENSIONS OFF)

# 添加编译选项
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  set(MY_DEBUG_OPTIONS -Wall -Werror -Wextra -Wshadow -pedantic -Wfloat-equal -Wno-unused-parameter -Wmain -fstack-usage -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  set(MY_DEBUG_OPTIONS -Wall -Wconversion -Wdouble-promotion -Weffc++ -Wextra -Wfloat-equal -Wmain -Wshadow -pedantic -g3)
  set(MY_RELEASE_OPTIONS -O2)

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(MY_DEBUG_OPTIONS /Wall /RTC)
  set(MY_RELEASE_OPTIONS /w3 /O2)
endif()

target_compile_options(testRunner PRIVATE
                                  "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(testRunner PRIVATE 
                                  "$<$<CONFIG:RELEASE>:{MY_RELEASE_OPTIONS>")
                                  
# 指定头文件路径
target_include_directories(testRunner PUBLIC
  ${TEST_EXTRA_INCLUDES}
  ${PROJECT_BINARY_DIR}/src
  PRIVATE ${PROJECT_SOURCE_DIR}/extern/cpp-httplib
  PRIVATE ${PROJECT_SOURCE_DIR}/extern/nlohmann-json/include/nlohmann
)

# 添加链接库
target_link_libraries(testRunner
  GTest::gtest_main
  GTest::gmock_main
  $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::SSL>
  $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:OpenSSL::Crypto>
  nlohmann_json::nlohmann_json
  ${TEST_EXTRA_LIBS}
  ${CMAKE_DL_LIBS}
)

install(TARGETS testRunner
  DESTINATION "${CMAKE_BINARY_DIR}/bin"
)

include(GoogleTest)
gtest_discover_tests(testRunner)


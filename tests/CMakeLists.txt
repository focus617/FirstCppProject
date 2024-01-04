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
  list(APPEND TEST_EXTRA_LIBS core)
  list(APPEND TEST_EXTRA_INCLUDES "${SOURCE_DIR}/core/include")
endif()

if (TEST_RESTFUL_LIB)
  message(STATUS "Building test for Restful Library...")
  add_subdirectory(restful)
  list(APPEND TEST_EXTRA_LIBS restful)
  list(APPEND TEST_EXTRA_INCLUDES 
      "${SOURCE_DIR}/restful/api"
      "${SOURCE_DIR}/restful/http"
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

# 添加编译选项
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_COMPILE_OPTIONS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_COMPILE_OPTIONS}")

target_compile_definitions(restful PUBLIC
    $<$<BOOL:${HTTPLIB_IS_USING_OPENSSL}>:CPPHTTPLIB_OPENSSL_SUPPORT>
)

# 指定头文件路径
target_include_directories(testRunner PUBLIC
  ${TEST_EXTRA_INCLUDES} 
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
)

include(GoogleTest)
gtest_discover_tests(testRunner)


# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# 查找当前目录下的所有源文件，并将名称保存到 _SOURCES 变量
aux_source_directory(. _TEST_SOURCES)

option(TEST_CORE_LIB "Test Core Lib" ON)
option(TEST_MATH_LIB "Test Math Lib" ON)

if (TEST_CORE_LIB)
    message(STATUS "Building test for Core Library...")
    add_subdirectory(core)
    set (TEST_EXTRA_LIBS ${TEST_EXTRA_LIBS} core)
endif()

if (TEST_MATH_LIB)
    message(STATUS "Building test for My Math Library...")
    add_subdirectory(math_lib)
    set (TEST_EXTRA_LIBS ${TEST_EXTRA_LIBS} xuzymath)
endif()

# 生成可执行程序
add_executable(testRunner ${_TEST_SOURCES})

# 添加链接库
target_link_libraries(testRunner
  GTest::gtest_main
  GTest::gmock_main
  ${TEST_EXTRA_LIBS}
)

include(GoogleTest)
gtest_discover_tests(testRunner)


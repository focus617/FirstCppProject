# additional target to perform clang-format run, requires clang-format
find_program(CLANG-FORMAT clang-format REQUIRED)

# find all C++ source files in the specified project directory
file(GLOB_RECURSE FORMAT_SOURCES
    LIST_DIRECTORIES false
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.h"
)
# file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.hpp *.h)
# foreach(SOURCE_FILE ${ALL_SOURCE_FILES})
#   string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
#   if(NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
#     list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
#   endif()
# endforeach()

add_custom_target(code_clangformat 
    COMMAND ${CLANG-FORMAT} -i ${FORMAT_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running clang-format source on ${SOURCE_DIR}"
)

# additional target to perform cppcheck run, requires cppcheck
find_program(CPPCHECK cppcheck REQUIRED)

# find all C++ source files in the specified project directory
file(GLOB_RECURSE ALL_SOURCE_FILES
    LIST_DIRECTORIES false
    "${SOURCE_DIR}/*.cpp"
    "${SOURCE_DIR}/*.hpp"
    "${SOURCE_DIR}/*.h"
)

# get all project files
# HACK this workaround is required to avoid qml files checking ^_^
# file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.hpp *.h)
# foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
#     string(FIND ${SOURCE_FILE} ${PROJECT_TRDPARTY_DIR} PROJECT_TRDPARTY_DIR_FOUND)
#     if (NOT ${PROJECT_TRDPARTY_DIR_FOUND} EQUAL -1)
#         list(REMOVE_ITEM ALL_SOURCE_FILES ${SOURCE_FILE})
#     endif ()
# endforeach ()

add_custom_target(code_cppcheck
    COMMAND ${CPPCHECK}
    --enable=warning,performance,portability,information,missingInclude
    --std=c++20
    --library=qt.cfg
    --template="[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)"
    --verbose
    --quiet
    --suppress=missingincludesystem
    ${ALL_SOURCE_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running cppcheck source on ${SOURCE_DIR}"
)

# 添加子目录
add_subdirectory(cpp)

#添加子目录的源文件给_SUB_SOURCES
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}  _SUB_SOURCES)

#将子目录源文件，追加给根目录_SOURCES变量
list(APPEND _SOURCES ${_SUB_SOURCES})

#设置根目录_SOURCES变量在子目录有效
set(_SOURCES ${_SOURCES} PARENT_SCOPE)

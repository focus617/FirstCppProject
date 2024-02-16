# 添加子目录
add_subdirectory(button)
add_subdirectory(drag)
add_subdirectory(input_field)
add_subdirectory(layout)
add_subdirectory(menu)
add_subdirectory(plot)
add_subdirectory(selection)
add_subdirectory(slider)
add_subdirectory(text)
add_subdirectory(visual)

#添加子目录的源文件给_SUB_SOURCES
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}  _SUB_SOURCES)

#将子目录源文件，追加给根目录_SOURCES变量
list(APPEND _SOURCES ${_SUB_SOURCES})

#设置根目录_SOURCES变量在子目录有效
set(_SOURCES ${_SOURCES} PARENT_SCOPE)




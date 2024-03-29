# check if Doxygen is installed
find_package(Doxygen REQUIRED doxygen)

if (DOXYGEN_FOUND)
  message(STATUS "Doxygen found.")
  
  # set input and output files
  # set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  # set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  # configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

  # add_custom_target(doc_doxygen
  #     COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
  #     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  #     COMMENT "Generating API documentation with Doxygen"
  #     VERBATIM
  # )
  
  # Cutomized Doxygen Options
  set(DOXYGEN_RECURSIVE YES)
  set(DOXYGEN_REFERENCED_BY_RELATION YES)
  set(DOXYGEN_REFERENCES_RELATION YES)
  set(DOXYGEN_SORT_BY_SCOPE_NAME YES)
  set(DOXYGEN_SORT_MEMBER_DOCS NO)
  set(DOXYGEN_SOURCE_BROWSER YES)
  set(DOXYGEN_STRIP_CODE_COMMENTS NO)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_TREEVIEW YES)
  set(DOXYGEN_HTML_OUTPUT ${CMAKE_BINARY_DIR}/doc_doxygen)

  list(APPEND DOXYGEN_HTML_EXTRA_STYLESHEET
      ${PROJECT_SOURCE_DIR}/docs/doxygen-awesome.css)
  list(APPEND DOXYGEN_HTML_EXTRA_STYLESHEET
      ${PROJECT_SOURCE_DIR}/docs/doxygen-awesome-sidebar-only.css)
  
  doxygen_add_docs(doc_doxygen
      ${PROJECT_SOURCE_DIR}/src
      ALL 
      COMMENT "Generating API documentation with Doxygen")

else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
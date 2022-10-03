# Helper function to join list of values with custom glue(separator)
function(join VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

# Helper function to pretty print a list of files.
function(pprint_files FILES)
  foreach(_TMP_FILE IN LISTS ${FILES})
    file(RELATIVE_PATH _TMP_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR} ${_TMP_FILE})
    list(APPEND _TMP_FILES ${_TMP_OUTPUT})
  endforeach()
  join("${_TMP_FILES}" "\n - " _TMP_PRINT_FILES)
  message(" - ${_TMP_PRINT_FILES}")
endfunction()

# Helper to find subdirectories under a directory. Returns a list.
macro(FIND_SUBDIRS OUTPUT DIR)
  file(GLOB CHILDREN ${DIR}/*)
  set(DIRLIST "")
  foreach(CHILD ${CHILDREN})
	if(IS_DIRECTORY ${CHILD})
      list(APPEND DIRLIST ${CHILD})
    endif()
  endforeach()
  set(${OUTPUT} ${DIRLIST})
endmacro()

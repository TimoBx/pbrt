# ********************************
# ************ gratin ************
# ********************************

# output executable directory
set(EXECUTABLE_OUTPUT_PATH ${GRATIN_EXE_DIR})

# include the src path 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# include all default paths
add_default_paths()

# create the executable (one single cpp file here)
add_executable(${GRATIN_EXE} main.cpp)

# link with libraries
link_to_default_libs(${GRATIN_EXE})
link_to_gratin_lib(${GRATIN_EXE})

# post-build (copy needed files)
add_custom_command(TARGET ${GRATIN_EXE}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${GRATIN_BIN_PATH}/nodes/custom
  COMMAND ${CMAKE_COMMAND} -E make_directory ${GRATIN_BIN_PATH}/data
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GRATIN_SRC_PATH}/nodes/custom ${GRATIN_BIN_PATH}/nodes/custom/.
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GRATIN_SRC_PATH}/data ${GRATIN_BIN_PATH}/data/.
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRATIN_SRC_PATH}/README.TXT ${GRATIN_BIN_PATH}/.
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRATIN_SRC_PATH}/LICENCE.MPL2 ${GRATIN_BIN_PATH}/.
  )

# installation rules 
install(TARGETS ${GRATIN_EXE} DESTINATION ${GRATIN_DIR_NAME})
install(DIRECTORY ${GRATIN_BIN_PATH}/data DESTINATION ${GRATIN_DIR_NAME} PATTERN .svn EXCLUDE)
install(DIRECTORY ${GRATIN_BIN_PATH}/nodes DESTINATION ${GRATIN_DIR_NAME} PATTERN .svn EXCLUDE)
install(FILES ${GRATIN_BIN_PATH}/README.TXT DESTINATION ${GRATIN_DIR_NAME})
install(FILES ${GRATIN_BIN_PATH}/LICENCE.MPL2 DESTINATION ${GRATIN_DIR_NAME})

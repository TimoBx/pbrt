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

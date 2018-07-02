# required - minimum cmake version
cmake_minimum_required(VERSION 2.6.2)

# include default modules and compiler options
include(${CMAKE_SOURCE_DIR}/cmake/gratin.conf.in)

# find and add default packages and directories (called only once)
find_default_packages()
add_default_paths()

# source lib and exe
add_subdirectory(src) 

# nodes
add_subdirectory(nodes) 

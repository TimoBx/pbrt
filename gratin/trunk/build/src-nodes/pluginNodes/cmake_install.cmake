# Install script for directory: /home/timothebarbaux/Documents/stage/gratin/trunk/src-nodes/pluginNodes

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "CMAKE_CXX_FLAGS_RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/poissonGrey/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/imgLoader/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/distanceMap/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/mvgLoader/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/imgCrop/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/multiImgLoader/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/gaussBlur/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/imgResize/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/fft2D/cmake_install.cmake")
  include("/home/timothebarbaux/Documents/stage/gratin/trunk/build/src-nodes/pluginNodes/GLSLoader/cmake_install.cmake")

endif()


# Install script for directory: /home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/openexr/IlmBase/IlmThread/libIlmThread.a")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadPool.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThread.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadSemaphore.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadMutex.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadNamespace.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadExport.h"
    "/home/timothebarbaux/Documents/stage/pbrt/src/ext/openexr/IlmBase/IlmThread/IlmThreadForward.h"
    )
endif()


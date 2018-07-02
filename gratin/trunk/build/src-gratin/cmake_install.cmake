# Install script for directory: /home/timothebarbaux/Documents/stage/gratin/trunk/src-gratin

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so.0.3.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "/usr/local/Gratin")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Gratin" TYPE SHARED_LIBRARY FILES
    "/home/timothebarbaux/Documents/stage/gratin/trunk/build/libgratin.so.0.3.2"
    "/home/timothebarbaux/Documents/stage/gratin/trunk/build/libgratin.so.0"
    "/home/timothebarbaux/Documents/stage/gratin/trunk/build/libgratin.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so.0.3.2"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/libgratin.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/timothebarbaux/Documents/stage/gratin/trunk/build:"
           NEW_RPATH "/usr/local/Gratin")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()


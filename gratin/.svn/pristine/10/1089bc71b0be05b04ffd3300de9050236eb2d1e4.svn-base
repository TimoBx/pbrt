# ********************************
# ********* libgratin-src ********
# ********************************

# all header files 
file(GLOB GRATIN_ALL_HEADERS misc/*.h core/*.h view/*.h)

# all source files 
file(GLOB GRATIN_SOURCES misc/*.cpp core/*.cpp view/*.cpp)

# all resource files 
set(GRATIN_RESOURCES src.qrc)

# find all qt object files in the header list
find_qt_object_files("${GRATIN_ALL_HEADERS}" GRATIN_HEADERS)

# use qt moc conversion
qt5_wrap_cpp(GRATIN_HEADERS_MOC ${GRATIN_HEADERS})

# use qt resource conversion
qt5_add_resources(GRATIN_RESOURCES_RCC ${GRATIN_RESOURCES})

# output library directory
set(LIBRARY_OUTPUT_PATH ${GRATIN_LIB_DIR})

# build shared libs
set(BUILD_SHARED_LIBS ON)
add_definitions(-DLIBRARY)

# create the library (using lib-name and all associated source-files)
add_library(${GRATIN_LIB} ${GRATIN_SOURCES} ${GRATIN_HEADERS_MOC} ${GRATIN_RESOURCES_RCC}) 

# link with external libraries
link_to_default_libs(${GRATIN_LIB})

# installation rules 
install(TARGETS ${GRATIN_LIB} DESTINATION ${GRATIN_DIR_NAME})

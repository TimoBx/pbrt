* before installing: 
make sure you have a GPU/graphics drivers capable to run (at least) OpenGL 4.1 applications 

* needed libraries:
qt5.2 (or more): http://qt-project.org/downloads (the .2 in qt5.2 is important)
eigen3: available as a linux package (I don't know for macos) (see http://eigen.tuxfamily.org/index.php?title=Main_Page )

* optionnal libraries 
openexr: available as a linux package (see http://www.openexr.com/ )
this library is used only to load/save high dynamic range images (exr format)

* compiling on linux (you need basic developement tools: cmake/make/g++)
cd path/to/gratin
mkdir build && cd build
cmake .. 
make -j8
make install 

if it does not find QT, add the following option to cmake:
cmake -DCMAKE_PREFIX_PATH="path/to/qt/5.2" ..

if it does not find eigen3, add the following option to cmake: 
cmake -DEIGEN3_INCLUDE_DIR="path/to/eigen3" ..

for changing the OpenGL version (default=4.2), use:
cmake -DCMAKE_CXX_FLAGS='-DOPENGL_MAJOR_VERSION=4 -DOPENGL_MINOR_VERSION=1' ..

for changing the installation path, add the following option to cmake:
cmake -DCMAKE_INSTALL_PREFIX="installation/path"

by default the qt path is "/opt/Qt5.2.1/5.2.1/gcc_64"
by default the opengl version is the 4.2

* launching gratin:
path/to/build/gratin -in path/to/gratin/pipeline.gra
or, if it was installed:
path/to/install/Gratin/gratin -in path/to/gratin/pipeline.gra

* cleaning/removing gratin
rm -rf path/to/install/Gratin (be carefull with that)

* to compile on os x, you should write something like this:
cmake .. -DCMAKE_CXX_COMPILER=/usr/local/bin/g++-4.7 -DCMAKE_C_COMPILER=/usr/local/bin/gcc-4.7 -DCMAKE_CXX_FLAGS='-DOPENGL_MAJOR_VERSION=4 -DOPENGL_MINOR_VERSION=1'

* notes about the interface 
*** all widgets
-- ctrl+n -> empty scene 
-- ctrl+o -> open scene
-- ctrl+s -> save scene
-- ctrl+q -> quit
-- ctrl+a -> select/unselect everything
-- ctrl+c -> copy selection
-- ctrl+v -> paste selection
-- ctrl+r -> reload shaders
-- p -> play anim
-- shift+p --> stop anim
-- shift+right -> next frame
-- shift+left -> previous frame
-- shift+up -> first frame
-- shift+down -> last frame

*** graph widget 
-- left click on the background -> unselect all 
-- left click on the background and move -> square selection
-- left click on a node -> select it
-- left click on a node + ctrl -> add to selection
-- left click on a node + shift -> remove from selection
-- left click on the output of a node and move -> make a new connection
-- left double click on a node -> show/highlight the node interface
-- right click on the backgroud and move -> translate
-- press space on a selected node -> add/remove it to the viewer/intraction widget
-- ctrl+g -> group the graph associated to the selected node 
-- ctrl+shift+g -> ungroup the node (need to be disconnected)
-- ctrl+w -> export the output of a selected node 
-- ctrl+shift+w -> export the output of a selected node for all frames
-- del key -> remove selection

*** graph widget + viewer widget 
-- center click and move -> zoom in/out
-- center scroll -> zoom in/out
-- ctrl + + -> zoom in
-- ctrl + - -> zoom out

*** viewer widget 
-- click on a node a first time -> select it
-- anything on top of this node -> interact / send it to this node for interaction

*** node list window
-- double-click on a node name -> add this node to the graph

*** node interface widget 
-- del key -> remove current widget 

*** animation widget 
-- del key -> remove selected param from the list 
-- right click on the backgroud and move -> translate
-- left click on the blue line -> modify current frame
-- left click on a control point and move -> modify control point position
-- ctrl + left click on a control point and move -> modify control point position with large controled steps
-- shift + left click on a control point and move -> modify control point position with small controled steps
-- ctrl + center click on a control point -> remove a control point 
-- ctrl + right click -> add a control point 
-- center button move/scroll -> scaling
-- ctrl + center button move/scroll -> horizontal scaling 
-- shift + center button move/scroll -> vertical scaling 


-- cleaning sources --
rm -rf path/to/gratin/build

or, if the cmake was done insource (be carefull with that):
make clean 
rm -rf `find . -name "Makefile*"`
rm -rf `find . -name "*.o"`
rm -rf `find . -name "*.so"`
rm -rf `find . -name "*~"`
rm -rf `find . -name "CMakeCache.txt"`
rm -rf `find . -name "cmake_install.cmake"`
rm -rf `find . -name "CMakeFiles"`
rm -rf `find . -name "moc_*"`
rm -rf `find . -name "ui_*"`
rm -rf `find . -name "*.qrc.depends"`
rm -rf `find . -name "__"`

*** CHANGES *** 
* Use of QT5 instead of QT4
-> use of QT5 OpenGL functions and context
-> remove dependency to glew


// TODO: 
// group nodes (minor-priority): when multiple outputs used, the size of a given output might appear different as another output
// group nodes (major-priority): changing generic node names inside a group do not actually change them properly inside the group node interface
   	       			 related: when loading a pipe containing a group, all generic nodes are named using their original names (i.e. imgGeneric instead of the name chosen by the user)

// add a search box for the nodes 
// double clic on a param should select and show the corredponding animation curve
// make it work on windows
--> std floor function and windows warning... 
// make it work on mac
--> retina screen 
// cmake should be done in a build directory
--> necessary files should be copied at the right place 
--> make install should work too
// better log file
// add nodes 

# making ubuntu binary package: 
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX="../install" ..
make -j8
make install 
cd ../install 
tar cvzf Gratin.tgz Gratin 
mv Gratin gratin-0.2 (version should correspond)
cd Gratin
dh_make -f ../Gratin.tgz
edit debian/control 
--> put X11 in section 
--> change priority by optional
--> change email if not good
--> add , libQt5Widgets (>= 5.2), libQt5Xml (>= 5.2), libopenexr6 (>= 1.6) in the build-depends
--> replace hompage text by http://gratin.gforge.inria.fr/
--> modify description and text 

edit the 3 other files (a script should be applied for all of it)
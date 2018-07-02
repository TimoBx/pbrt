* needed libs:
qt (4)
eigen3
opengl (gl/glu)
glew 
openexr(optional)

* install external libs:
sudo apt-get install libqt4-dev libeigen3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev libopenexr-dev 

* compiling:
cd path/to/gratin
cmake . && make -j8

* use gratin:
./gratin -in path/to/gratin/pipeline.gratin

* interface 
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
-- double right click on a node -> add/remove it to the viewer/intraction widget
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
-- anything on top of a node -> send it to this node for interaction

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


-- perso notes --
Compiling using qtcreator:
add -j8 in compilation steps
add path to gl/glu/glew dll in the PATH variable (projects/execute)

on linux:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib

big clean:
make clean 
rm -rf `find . -name "Makefile*"`
rm -rf `find . -name "*.o"`
rm -rf `find . -name "*.so"`
rm -rf `find . -name "*~"`
rm -rf `find . -name "CMakeCache.txt"`
rm -rf `find . -name "cmake_install.cmake"`
rm -rf `find . -name "CMakeFiles"`

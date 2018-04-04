# CMake generated Testfile for 
# Source directory: /home/timothebarbaux/Documents/stage/pbrt/src/ext/ptex/src/tests
# Build directory: /home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(wtest "/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/wtest")
add_test(rtest "/usr/bin/cmake" "-DOUT=/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/rtest.out" "-DDATA=/home/timothebarbaux/Documents/stage/pbrt/src/ext/ptex/src/tests/rtestok.dat" "-DCMD=/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/rtest" "-P" "/home/timothebarbaux/Documents/stage/pbrt/src/ext/ptex/src/tests/compare_test.cmake")
add_test(ftest "/usr/bin/cmake" "-DOUT=/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/ftest.out" "-DDATA=/home/timothebarbaux/Documents/stage/pbrt/src/ext/ptex/src/tests/ftestok.dat" "-DCMD=/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/ftest" "-P" "/home/timothebarbaux/Documents/stage/pbrt/src/ext/ptex/src/tests/compare_test.cmake")
add_test(halftest "/home/timothebarbaux/Documents/stage/pbrt/build/src/ext/ptex/src/tests/halftest")

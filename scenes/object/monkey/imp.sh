#!/bin/bash

MY_MESSAGE=$'Steps:\n \n1) Imp map generation; \n2) Rendered scene using the imp map; \n3) Open the imp map; \n4) Open the rendered image.\n\n\n'
echo "$MY_MESSAGE"

p=".pbrt"
e=".exr"
i="impmap_"
INPUT=$1$p
IMP=$i$1$e
OUTPUT=$1$e

../../../build/pbrt $INPUT --importance --quiet
../../../build/pbrt $INPUT --quiet
exrdisplay $IMP
exrdisplay $OUTPUT

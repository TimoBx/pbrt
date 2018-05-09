#!/bin/bash
# 
# MY_MESSAGE=$'\nSteps of this script:\n
# 1) Generate the importance map;
# 2) Render the image using the importance map map;
# 3) Open the importance map;
# 4) Open the rendered image.\n\n\n'
# echo "$MY_MESSAGE"

p=".pbrt"
e=".exr"
i="impmap_"
INPUT=$1$p
IMP=$i$1$e
OUTPUT=$1$e

rm $IMP
rm $OUTPUT

echo "   ---  GENERATING THE IMP MAP  ---   "
../../../build/pbrt $INPUT --importance --quiet
# echo "   ---  RENDERING THE IMAGE USING THE IMP MAP  ---   "
# ../../../build/pbrt $INPUT --quiet
echo "   ---  IMP MAP  ---   "
exrdisplay $IMP
# echo "   ---  RENDERED IMAGE  ---   "
# exrdisplay $OUTPUT

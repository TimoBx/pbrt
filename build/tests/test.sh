#!/bin/bash

p=".pbrt"
e=".exr"
i="impmap_"

INPUT=$1$p
IMP=$i$1$e
OUTPUT=$1$e


if [ "$1" = "" ]
then
        printf "\nCan you please give me a file name (without its extension) ?\n\n"


else


if [ "$2" != "" ]
then
          IMP=$i$1_$2$e
          OUTPUT=$1_$2$e
          m="--matchange"
fi


if [ -f $IMP ]
then
	rm $IMP
fi

if [ -f "$OUTPUT" ]
then
	rm $OUTPUT
fi

printf "   \n\n"
# printf "---  GENERATING THE IMP MAP  ---  \n\n "
../../../build/pbrt $INPUT --importance --quiet $m $2
# printf "   \n\n---  RENDERED IMAGE  ---   \n\n"
if [ -f $OUTPUT ]
then
  exrdisplay $OUTPUT
fi

# printf "   \n\n---  IMP MAP  ---   \n\n"
if [ -f $IMP ]
then
exrdisplay $IMP
fi


fi

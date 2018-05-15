#!/bin/bash

p=".pbrt"
e=".exr"
i="impmap_"

INPUT=$1$p
IMP=$i$1$2$e
OUTPUT=$1$2$e


if [ "$1" = "" ]
then
        printf "\nCan you please give me a file name (without its extension) ?\n\n"


else

if [ -f $IMP ]
then
	rm $IMP
fi

if [ -f "$OUTPUT" ]
then
	rm $OUTPUT
fi

if [ "$2" != "" ]
then
        m="--matchange"
	printf "\n\nSetting the default material to $2 !\n\n"
fi

printf "   \n\n---  GENERATING THE IMP MAP  ---  \n\n "
../../../build/pbrt $INPUT --importance --quiet $m $2
printf "   \n\n---  RENDERED IMAGE  ---   \n\n"
exrdisplay $OUTPUT
printf "   \n\n---  IMP MAP  ---   \n\n"
exrdisplay $IMP

fi

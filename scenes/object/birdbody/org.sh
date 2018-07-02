#!/bin/bash

p=".pbrt"
e=".exr"
png=".png"

USAGE="\n\org.sh usage: ./org.sh <filename without extension> <new mat if needed, else type \"default\"> <light map filename for lighting design>\n\n"
NOLIGHTMAP="\nERROR SYSTEM!\n Your light map does not exist! Your silly mistake will result in your computer self-destructing, in 3... 2... 1...\n\n Just kidding, just enter a valid file name por favor!\n\n "

if [ "$1" = "" ] || [ "$1" = "usage" ] || [ "$2" = "" ]
then
        printf "$USAGE"
        exit 1
fi


m=""
m2=""
mat=""
if [ "$2" != "default" ]
then
          m=_$2
          m2=$2
          mat="--matchange"
fi

map=""
lm="lightmap"
if [ "$3" != "" ]
then
    map=$3
else
    map=$lm$m$png
fi


INPUT=$1$p
OUTPUT=$1$m$e


if [ ! -f $map ]
then
  printf "$NOLIGHTMAP"
  exit 1
fi

../../../build/pbrt $INPUT --quiet $mat $m2


if [ -f $OUTPUT ] && [ -f $map ]
then
    pfsv $map $OUTPUT
fi


ld="ld"
m=_$2
dir=$ld$m

if [ ! -d $dir ]
then
    mkdir $dir
fi


nb=$(find $dir -maxdepth 1 -type d | wc -l)

newdir=$ld$nb
s="/"
path=$dir$s$newdir


mkdir $path


mv $map $path
mv $OUTPUT $path

#!/bin/bash

p=".pbrt"
e=".exr"

if [ "$1" = "" ]
then
        printf "\nCan you please give me a file name (without its extension) ?\n\n"
else

m=""
if [ "$2" != "" ]
then
          m=_$2
          mat="--matchange"
fi

INPUT=$1$p
OUTPUT=$1$m$e

R0="impmapR0_"
RX="impmapRX_"
R="impmapR_"
TX="impmapTX_"
TT0="impmapTT0_"
TT="impmapTT_"
TTX="impmapTTX_"
TRT="impmapTRT_"
TRTX="impmapTRTX_"
TRT0="impmapTRT0_"
ALL="impmapALL_"

IMP=$ALL$1$m$e
IMP1=$R$1$m$e
IMP2=$TX$1$m$e
IMP3=$R0$1$m$e
IMP4=$RX$1$m$e
IMP5=$TT$1$m$e
IMP6=$TT0$1$m$e
IMP7=$TTX$1$m$e
IMP8=$TRT$1$m$e
IMP9=$TRT0$1$m$e
IMP10=$TRTX$1$m$e


printf "   \n\n"
../../../build/pbrt $INPUT --importance --quiet $mat $2


printf "   \n\n Here are the images created and displayed :\n\n"
echo $IMP
echo $IMP1
echo $IMP2
echo $IMP3
echo $IMP4
echo $IMP5
echo $IMP6
echo $IMP7
echo $IMP8
echo $IMP9
echo $IMP10

printf "   \n\n"

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

# printf "   \n\n---  IMP MAP R  ---   \n\n"
if [ -f $IMP1 ]
then
exrdisplay $IMP1
fi

# printf "   \n\n---  IMP MAP TX ---   \n\n"
if [ -f $IMP2 ]
then
exrdisplay $IMP2
fi

# printf "   \n\n---  IMP MAP R0 ---   \n\n"
if [ -f $IMP3 ]
then
  exrdisplay $IMP3
fi

# printf "   \n\n---  IMP MAP RX  ---   \n\n"
if [ -f $IMP4 ]
then
  exrdisplay $IMP4
fi

# printf "   \n\n---  IMP MAP TT ---   \n\n"
if [ -f $IMP5 ]
then
  exrdisplay $IMP5
fi

# printf "   \n\n---  IMP MAP TT0 ---   \n\n"
if [ -f $IMP6 ]
then
  exrdisplay $IMP6
fi

# printf "   \n\n---  IMP MAP TTX ---   \n\n"
if [ -f $IMP7 ]
then
  exrdisplay $IMP7
fi

# printf "   \n\n---  IMP MAP TRT ---   \n\n"
if [ -f $IMP8 ]
then
  exrdisplay $IMP8
fi

# printf "   \n\n---  IMP MAP TRT0 ---   \n\n"
if [ -f $IMP9 ]
then
  exrdisplay $IMP9
fi

# printf "   \n\n---  IMP MAP TRTX ---   \n\n"
if [ -f $IMP10 ]
then
  exrdisplay $IMP10
fi




fi

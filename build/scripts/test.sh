#!/bin/bash

p=".pbrt"
e=".exr"

USAGE="\ntest.sh usage: ./test.sh <filename without extension> <new mat if needed, else type \"default\"> <if needed, one or two mask filenames>\n\n"

if [ "$1" = "" ] || [ "$1" = "usage" ] || [ "$2" = "" ]
then
        echo $USAGE
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

maskOpt=""
mask1=""
mask2=""
if [ "$3" != "" ]
then
    maskOpt="--mask"
    mask1=$3
fi
if [ "$4" != "" ]
then
    maskOpt="--mask2"
    mask2=$4
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

../../../build/pbrt $INPUT --importance --quiet $mat $m2 $maskOpt $mask1 $mask2



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

# printf "   \n\n---  IMP MAPS  ---   \n\n"
if [ -f $IMP1 ] && [ -f $IMP2 ] && [ -f $IMP4 ] && [ -f $IMP5 ] && [ -f $IMP8 ]
then
    pfsv $IMP1 $IMP4 $IMP2 $IMP5 $IMP8
fi



DEBUG=false
if ${DEBUG}; then
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

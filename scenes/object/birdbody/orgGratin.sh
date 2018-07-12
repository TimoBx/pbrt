#!/bin/bash
e=".exr"
a="active"
ALL="impmapALL_"
R="impmapR_"
TX="impmapTX_"
R0="impmapR0_"
RX="impmapRX_"
TT="impmapTT_"
TT0="impmapTT0_"
TTX="impmapTTX_"
TRT="impmapTRT_"
TRT0="impmapTRT0_"
TRTX="impmapTRTX_"


USAGE="\n\orgGratin.sh usage: ./orgGratin.sh <filename without extension> <new mat if needed, else type \"default\">\n\n"
NOMAPS="\nERROR SYSTEM!\nThe maps you specified don't exist... There's at least one missing. Please generate them all, or specify another set of imp maps.\n\n "

if [ "$1" = "" ] || [ "$1" = "usage" ] || [ "$2" = "" ]
then
        printf "$USAGE"
        exit 1
fi


m=""
if [ "$2" != "default" ]
then
          m=_$2
fi

i0=$1$m$e
i1=$ALL$1$m$e
i2=$R$1$m$e
i3=$TX$1$m$e
i4=$R0$1$m$e
i5=$RX$1$m$e
i6=$TT$1$m$e
i7=$TT0$1$m$e
i8=$TTX$1$m$e
i9=$TRT$1$m$e
i10=$TRT0$1$m$e
i11=$TRTX$1$m$e

if [ ! -f $i0 ] || [ ! -f $i1 ] || [ ! -f $i2 ] || [ ! -f $i3 ] || [ ! -f $i4 ] || [ ! -f $i5 ] || [ ! -f $i6 ] || [ ! -f $i7 ] || [ ! -f $i8 ] || [ ! -f $i9 ] || [ ! -f $i10 ] || [ ! -f $i11 ]
then
  printf "$NOMAPS"
  exit 1
fi

n1=$ALL$a$e
n2=$R$a$e
n3=$TX$a$e
n4=$R0$a$e
n5=$RX$a$e
n6=$TT$a$e
n7=$TT0$a$e
n8=$TTX$a$e
n9=$TRT$a$e
n10=$TRT0$a$e
n11=$TRTX$a$e


path="../../../../gratin/trunk/data/maps"

cp $i1 $path
cp $i2 $path
cp $i3 $path
cp $i4 $path
cp $i5 $path
cp $i6 $path
cp $i7 $path
cp $i8 $path
cp $i9 $path
cp $i10 $path
cp $i11 $path

cd "$path"

mv $i1 $n1
mv $i2 $n2
mv $i3 $n3
mv $i4 $n4
mv $i5 $n5
mv $i6 $n6
mv $i7 $n7
mv $i8 $n8
mv $i9 $n9
mv $i10 $n10
mv $i11 $n11

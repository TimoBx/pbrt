#!/bin/bash


if [ "$1" = "" ]
then
        printf "\nCan you please give me a file name (without its extension) ?\n\n"


else

MATTE="matte"
MIRROR="mirror"
PLASTIC="plastic"


printf "   \n\n\n---  TEST WITH MATTE MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 $MATTE

printf "   \n\n\n---  TEST WITH MIRROR MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 $MIRROR

printf "   \n\n\n---  TEST WITH PLASTIC MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 $PLASTIC

fi

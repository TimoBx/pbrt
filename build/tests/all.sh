#!/bin/bash


if [ "$1" = "" ]
then
        printf "\nCan you please give me a file name (without its extension) ?\n\n"


else



printf "   \n\n\n---  TEST WITH MATTE MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 matte

printf "   \n\n\n---  TEST WITH MATTEYELLOW MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 matteyellow

printf "   \n\n\n---  TEST WITH GLASS MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 glass

printf "   \n\n\n---  TEST WITH MIRROR MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 mirror

printf "   \n\n\n---  TEST WITH MIRROR01 MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 mirror01

printf "   \n\n\n---  TEST WITH METAL MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 metal

printf "   \n\n\n---  TEST WITH PLASTIC MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 plastic

printf "   \n\n\n---  TEST WITH PLASTIC001 MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 plastic001

printf "   \n\n\n---  TEST WITH PLASTIC09 MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 plastic09

printf "   \n\n\n---  TEST WITH TRANSLUCENT MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 translucent

printf "   \n\n\n---  TEST WITH UBER MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 uber

printf "   \n\n\n---  TEST WITH CUSTOM MATERIAL  ---  \n\n\n "
../../../build/tests/test.sh $1 custom

fi

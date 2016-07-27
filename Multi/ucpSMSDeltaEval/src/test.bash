#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./MOUCPEANorme  --inputFile="./input-multi/probleme60.tsp" -G500   --pCross=0.7217  --pMut=0.4906 --popSize=20 --nbLambda=3 --mut1Rate=0.3737 --mut2Rate=0.1797 --mut3Rate=0.7185 --mut4Rate=0.8574 --mut5Rate=0.7091  --mut5RateMO=0.1487 --pVectMut=0.1097 --nbU=5 --mut2HDRate=0.9792 --mut2HRate=0.5826 --mutMORate=0.7534 --repet=4 --nbOffspring=13  >>Test60/$var
STDOUT="Test60/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test60/result
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done
#--pCross 0.7217 --pMut 0.4906 --pMut1 0.3737 --pMut2 0.1797 --pHMut2 0.5826 --nbU 5 --pMut3 0.7185 --pMut4 0.8574 --pMut5 0.7091 --nbLambda 3 --pMut5MO 0.1487 --pVectMut 0.1097 --mutHDRate 0.9792 --mutMORate 0.7534 --repet 4 --nbOffspring 13

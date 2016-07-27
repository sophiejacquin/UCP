#!/bin/bash
i=2
s=55555551
while [ $i -le 20 ]; do 
var=testE${i}
./MOUCPEANorme  --inputFile="probleme100-2.tsp"  --pCross=0.91  --pMut=0.8297 --popSize=20 --nbLambda=5  --cross2Rate=0.728 --cross3Rate= 0.6039 --mut1Rate=0.7514 --mut2Rate=0.1575 --mut3Rate=0.5141 --mut4Rate=0.9289 --mut5Rate=0.6869 --mut5RateMO=0.1704 --pVectMut=0.0855 --nbU=6 --mut2HDRate=0.7745 --mutMORate=0.6085 --repet=3  --seed=$s >>Test100-2/$var
STDOUT="Test100-2/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test100-2/result
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done


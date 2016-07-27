#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./UCP  --inputFile="../input/probleme60.tsp"   --pCross1=0.156 --pWCross=0.8077 --pMut=0.2197 --pMut1=0.4904 --pWMut=0.6417 --pCross=0.8934 --popSize=67 -G300000 --tempsMax=400 --seed=$s >>Test60/$var
STDOUT="Test60/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test60/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./UCP --inputFile="../input/probleme80.tsp"   --pCross1=0.156 --pWCross=0.8077 --pMut=0.2197 --pMut1=0.4904 --pWMut=0.6417 --pCross=0.8934 --popSize=67 -G300000 --tempsMax=400 --seed=$s >>Test80/$var
STDOUT="Test80/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test80/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./UCP @/home/jacquin/Bureau/UCP/param/UCP10.param --inputFile="/home/jacquin/Bureau/UCP/input/probleme10.tsp"   --pCross=0.5710 --pCross1=0.0206  --pWCross=0.7335  --pMut=0.6004 --pMut1=0.2706  --pWMut=0.5666 --popSize=66 -G300000 --seed=$s >>Test10/$var
STDOUT="Test10/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test10/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

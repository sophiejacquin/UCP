#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./UCP @/home/jacquin/Bureau/UCP/param/UCP20.param --inputFile="/home/jacquin/Bureau/UCP/input/probleme40.tsp"   --pCross1=0.3137 --pWCross=0.7701 --pMut=0.8163 --pMut1=0.2499 --pWMut=0.3644 --pCross=0.9174 --popSize=41 -G300000 --tempsMax=100 --seed=$s >>Test40/$var
STDOUT="Test40/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test40/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

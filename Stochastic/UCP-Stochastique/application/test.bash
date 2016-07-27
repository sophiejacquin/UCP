#!/bin/bash
i=1
s=1384347315
while [ $i -le 10 ]; do 
var=testE${i}
./UCP  --inputFile="/home/jacquin/Bureau/UCP/input/probleme100.tsp"  -G3000  -P100 -w1 --seed=$s >>Test/$var
STDOUT="Test/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

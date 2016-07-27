#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./MOUCPEANorme  --inputFile="./input-multi/probleme60.tsp" -G500   --pCross=0.8920  --pMut=0.7785 --popSize=20 --nbLambda=4 --mut1Rate=0.1852 --mut2Rate=0.9720 --mut3Rate=0.5707 --mut4Rate=0.1584 --mut5Rate=0.8675  --mut5RateMO=0.8570 --pVectMut=0.7461 --nbU=5 --mut2HDRate=0.5464 --mut2HRate=0.3204 --mutMORate=0.4597 --repet=4 >>Test60/$var
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
./MOUCPEANorme  --inputFile="./input-multi/probleme80.tsp" -G500   --pCross=0.4971  --pMut=0.4507 --popSize=20 --nbLambda=4 --mut1Rate=0.1234 --mut2Rate=0.1250 --mut3Rate=0.8022 --mut4Rate=0.7023 --mut5Rate=0.9470  --mut5RateMO=0.8833 --pVectMut=0.0616 --nbU=6 --mut2HDRate=0.1302 --mut2HRate=0.0090 --mutMORate=0.4492 --repet=4 >>Test80/$var
STDOUT="Test80/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test80/result
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

#./MOUCPEANorme  --inputFile="./input-multi/probleme10.tsp" --time_max=120   --pCross=0.6241  --pMut=0.6628 --popSize=20 --nbLambda=8 --mut1Rate=0.5000 --mut2Rate=0.1143 --mut3Rate=0.9297 --mut4Rate=0.3286 --mut5Rate=0.4863  --mut5RateMO=0.9043 --pVectMut=0.5646 --nbU=9 --mut2HDRate=0.5380 --mut2HRate=0.3380 --mutMORate=0.3809 --repet=4 >>Test10/$var

#./MOUCPEANorme  --inputFile="./input-multi/probleme20.tsp" --time_max=240   --pCross=0.5303  --pMut=0.6449 --popSize=20 --nbLambda=4 --mut1Rate=0.4078 --mut2Rate=0.2567 --mut3Rate=0.2517 --mut4Rate=0.8115 --mut5Rate=0.3328  --mut5RateMO=0.6845 --pVectMut=0.4730 --nbU=8 --mut2HDRate=0.4137 --mut2HRate=0.3045 --mutMORate=0.2696 --repet=3 >>Test20/$var
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./MOUCPEANorme --inputFile="probleme100-2.tsp"    -G500  --pCross=0.3229  --pMut=0.9761 --popSize=20 --nbLambda=5 --mut1Rate=0.3899 --mut2Rate=0.4967 --mut3Rate=0.8309 --mut4Rate=0.5251 --mut5Rate=0.847  --mut5RateMO=0.2428 --pVectMut=0.2572 --nbU=6 --mut2HDRate=0.2894 --mut2HRate=0.5347 --mutMORate=0.2669 --repet=3  --seed=$s >>Test100-2/$var
STDOUT="Test100-2/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test100-2/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done

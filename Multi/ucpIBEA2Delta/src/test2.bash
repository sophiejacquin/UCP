#!/bin/bash
i=41
s=555095879
while [ $i -le 60 ]; do 
var=testE${i}
./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme20.tsp"    -G300000  -P62 -w1 -C0.7681 -x0.5998 -y0.9984 -M0.6511 -10.6142 -20.8959 -30.099 -50.7238 -u5 --seed=$s >>Test20/$var
STDOUT="Test20/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test40/result
echo "$COST" $var fait
let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme100bis.tsp"    -G30000  -P44 -w1 -C0.9701 -x0.6239 -y0.7124 -M0.5144 -10.6679 -10.147 -30.8753 -50.1316 -u4 --seed=$s >>Test100/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme80.tsp"    -G30000  -P45 -w1 -C0.8719 -x0.605 -y0.627 -M0.1606 -10.0871 -10.4416 -30.3349 -50.9379 -u4 --seed=$s >>Test80/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme60.tsp"    -G30000  -P65 -w1 -C0.9026 -x0.8768 -y0.1922 -M0.3962 -10.2515 -10.1506 -30.5696 -50.9732 -u4 --seed=$s >>Test60/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme40.tsp"    -G30000  -P29 -w1 -C0.7506 -x0.6779 -y0.35 -M0.7628 -10.2629 -10.2395 -30.8297 -50.6339 -u4 --seed=$s >>Test40/$var
#s=1384347315
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme20.tsp"    -G300000  -P83 -w1 -C0.933 -x0.7161 -y0.8165 -M0.7834 -10.7387 -10.6406 -30.6399 -50.5165 -u4 --seed=$s >>Test10/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme20.tsp"    -G300000  -P62 -w1 -C0.7681 -x0.5998 -y0.9984 -M0.6511 -10.6142 -20.8959 -30.099 -50.7238 -u4 --seed=$s >>Test10/$var

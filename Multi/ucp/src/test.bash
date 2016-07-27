#!/bin/bash
i=1
s=5555555
while [ $i -le 20 ]; do 
var=testE${i}
./UCPEA  --inputFile="/home/jacquin/Bureau/UCP-Deterministe/input/probleme100.tsp"    -G30000  -P44 -w1 -C0.8304 -x0.4123 -y0.3561 -M0.0389 -10.8824 -20 -30.8527 -50 -u4 --temps=200 --seed=$s >>Test100/NH/$var
STDOUT="Test100/NH/$var"
COST=$(tail -1 ${STDOUT} | cut -d ':' -f 2)
echo "$COST" >>Test100/resultNH
echo "$COST" $var fait

let $[ s+=100 ]
let $[ i+=1 ] # incremente i de 1 a chaque boucle 
done
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme100bis.tsp"    -G30000  -P44 -w1 -C0.9701 -x0.6239 -y0.7124 -M0.5144 -10.6679 -10.147 -30.8753 -50.1316 -u4 --seed=$s >>Test100/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme80.tsp"    -G30000  -P45 -w1 -C0.8719 -x0.605 -y0.627 -M0.1606 -10.0871 -10.4416 -30.3349 -50.9379 -u4 --seed=$s >>Test80/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme60.tsp"    -G30000  -P65 -w1 -C0.9026 -x0.8768 -y0.1922 -M0.3962 -10.2515 -10.1506 -30.5696 -50.9732 -u4 --seed=$s >>Test60/$var
#s=1384347315
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme10.tsp"    -G300000  -P32 -w1 -C0.6775 -x0.5726 -y0.1637 -M0.4146 -10.7387 -10.1672 -30.8562 -20.6452 -50.2312 -u3 --seed=$s >>Test10/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme20.tsp"    -G300000  -P62 -w1 -C0.7681 -x0.5998 -y0.9984 -M0.6511 -10.6142 -20.8959 -30.099 -50.7238 -u4 --seed=$s >>Test20/$var
#./UCPEA  --inputFile="/home/jacquin/Bureau/UCP/input/probleme40.tsp"    -G30000  -P29 -w1 -C0.7506 -x0.6779 -y0.35 -M0.7628 -10.2629 -20.2395 -30.8297 -50.6339 -u4 --seed=$s >>Test40/$var

#!/bin/bash
PCROSS=$2
PCROSS2=$4
PCROSS3=$6
PMUT=$8
PMUT1=${10}
PMUT2=${12}
PHMUT2=${14}
NBU=${16}
PMUT3=${18}
PMUT4=${20}
PMUT5=${22}
NBL=${24}
POP=${26}
PMUT5MO=${28}
PVECT=${30}
MUTHD=${32}
MUTMO=${34}
REPET=${36}

CMD="/Users/sophiejacquin/Desktop/ucp/src/MOUCPEANorme   --inputFile=/Users/sophiejacquin/Desktop/Lucien-UCP/ucp-decoder/Lucien-UCP/Code/UCP-Multi/input-multi/probleme80.tsp"

$CMD  --pCross=$PCROSS  --pMut=$PMUT --popSize=$POP --nbLambda=$NBL  --cross2Rate=$PCROSS2 --cross3Rate=$PCROSS3 --mut1Rate=$PMUT1 --mut2Rate=$PMUT2 --mut3Rate=$PMUT3 --mut4Rate=$PMUT4 --mut5Rate=$PMUT5 --mut5RateMO=$PMUT5MO --pVectMut=$PVECT --nbU=$NBU --mut2HDRate=$MUTHD --mutMORate=$MUTMO --repet=$REPET 

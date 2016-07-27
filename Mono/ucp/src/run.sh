#!/bin/bash
PCROSS=$2
PCROSS2=$4
PCROSS3=$6
PMUT=$8
PMUT1=${10}
PMUT2=${12}
NBU=${14}
PMUT3=${16}
PMUT4=${18}
PMUT5=${20}
NBL=${22}
PMUT5MO=${24}
PVECT=${26}
MUTHD=${28}
MUTMO=${30}
REPET=${32}

CMD="/home/jacquin/Bureau/ucp/src/MOUCPEANorme   --inputFile=../input-multi/probleme100.tsp"

$CMD  --pCross=$PCROSS  --pMut=$PMUT --popSize=20 --nbLambda=$NBL  --cross2Rate=$PCROSS2 --cross3Rate=$PCROSS3 --mut1Rate=$PMUT1 --mut2Rate=$PMUT2 --mut3Rate=$PMUT3 --mut4Rate=$PMUT4 --mut5Rate=$PMUT5 --mut5RateMO=$PMUT5MO --pVectMut=$PVECT --nbU=$NBU --mut2HDRate=$MUTHD --mutMORate=$MUTMO --repet=$REPET 

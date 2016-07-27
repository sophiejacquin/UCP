#!/bin/bash
PCROSS=$2
PCROSS2=$4
PMUT=$6
PMUT1=${8}
PMUT2=${10}
PHMUT2=${12}
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
NBOFF=${34}

CMD="../MOUCPEANorme --inputFile=../input-multi/probleme20.tsp"

$CMD  --pCross=$PCROSS  --pMut=$PMUT --popSize=20 --nbLambda=$NBL  --cross2Rate=$PCROSS2 --mut1Rate=$PMUT1 --mut2Rate=$PMUT2 --mut3Rate=$PMUT3 --mut4Rate=$PMUT4 --mut5Rate=$PMUT5 --mut5RateMO=$PMUT5MO --pVectMut=$PVECT --nbU=$NBU --mut2HDRate=$MUTHD --mut2HRate=$PHMUT2 --mutMORate=$MUTMO --repet=$REPET --nbOffspring=$NBOFF --time_max=240
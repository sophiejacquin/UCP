#!/bin/bash
PCROSS=$2
PMUT=$4
PMUT1=${6}
PMUT2=${8}
PHMUT2=${10}
NBU=${12}
PMUT3=${14}
PMUT4=${16}
PMUT5=${18}
NBL=${20}
PMUT5MO=${22}
PVECT=${24}
MUTHD=${26}
MUTMO=${28}
REPET=${30}
NBO=${34}

CMD="../MOUCPEANorme --inputFile=../input-multi/probleme10.tsp"

$CMD  --pCross=$PCROSS  --pMut=$PMUT --popSize=20 --nbLambda=$NBL --mut1Rate=$PMUT1 --mut2Rate=$PMUT2 --mut3Rate=$PMUT3 --mut4Rate=$PMUT4 --mut5Rate=$PMUT5 --mut5RateMO=$PMUT5MO --pVectMut=$PVECT --nbU=$NBU --mut2HDRate=$MUTHD --mut2HRate=$PHMUT2 --mutMORate=$MUTMO --repet=$REPET --nbOffspring=$NBOFF --time_max=120

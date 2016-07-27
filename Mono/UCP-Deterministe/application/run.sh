#!/bin/bash
PCROSS1=$2
PWCROSS=$4
PMUT=$6
PMUT1=$8
PWMUT=${10}
PCROSS=${12}
POP=${14}


CMD="/home/jacquin/Bureau/UCP/application/UCP @/home/jacquin/Bureau/UCP/param/UCP20.param --inputFile=/home/jacquin/Bureau/UCP/input/probleme80.tsp"

$CMD  --pCross=$PCROSS --pCross1=$PCROSS1  --pWCross=$PWCROSS  --pMut=$PMUT --pMut1=$PMUT1  --pWMut=$PWMUT --popSize=$POP --maxGen=1000000 --tempsMax=200


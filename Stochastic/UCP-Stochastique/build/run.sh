#!/bin/bash
PCROSS=$2
PWCROSS=$4
PMUT=$6
PWMUT=$8

CMD="/home/jacquin/Bureau/UCP/build/UCP @/home/jacquin/Bureau/UCP/param/UCP4.param --inputFile=/home/jacquin/Bureau/UCP/input/probleme10.tsp --scenariosFolder=/home/jacquin/Bureau/UCP/input/scenarios --scenariosNum=20"

$CMD  --pCross=$PCROSS  --pWCross=$PWCROSS  --pMut=$PMUT  --pWMut=$PWMUT


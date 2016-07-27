#!/bin/bash
PCROSS=$2
PWCROSS=$4
PMUT=$6
PWMUT=$8

CMD="/home/jacquin/Bureau/UCP/application/UCP @/home/jacquin/Bureau/UCP/param/UCP4.param --inputFile=/home/jacquin/Bureau/UCP/input/probleme10.tsp"

$CMD  --pCross=$PCROSS  --pWCross=$PWCROSS  --pMut=$PMUT  --pWMut=$PWMUT


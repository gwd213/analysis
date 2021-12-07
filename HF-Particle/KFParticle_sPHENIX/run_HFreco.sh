#!/bin/bash

export HOME=/sphenix/u/${LOGNAME}
source /opt/sphenix/core/bin/sphenix_setup.sh -n new 
'''
export SPHENIX=$HOME/sPHENIX
export MYINSTALL=$SPHENIX/install
export LD_LIBRARY_PATH=$MYINSTALL/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$MYINSTALL/include:$ROOT_INCLUDE_PATH

source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
'''

nEvents=0
echo running: run_HFreco.sh $*
echo arg1 \(track inputFile\) : $1
echo arg2 \(vertex inputFile\) : $2
echo arg2 \(truth inputFile\) : $3
echo arg3 \(calo inputFile\) : $4
echo arg2 \(traker hit inputFile\) : $5
echo arg2 \(BBC G4hit inputFile\) : $6
echo arg2 \(tracker G4hit inputFile\) : $7
echo Pileup is not implemented yet
root.exe -q -b Fun4All_HFreco.C\(\"$1\",\"$2\",\"$3\",\"$4\",\"\",$nEvents\)
echo Script done

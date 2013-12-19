#!/bin/bash

# for each bank file pbk in /gpfs/ddn/cms/user/martini/AM/PatternBanks/BE_5D/Eta6_Phi8 
# prints the size

cd /gpfs/ddn/cms/user/martini/AM/CMSSW_6_1_2_SLHC6_patch1/src/amsimulation

for f in /gpfs/ddn/cms/user/martini/AM/PatternBanks/BE_5D/Eta6_Phi8/*lo?midhig*.pbk
do
	n=`./AMSimulation --printBank --bankFile ${f} | wc -l`
	fb=`basename ${f}` 
    echo "${fb} = ${n}" 
done
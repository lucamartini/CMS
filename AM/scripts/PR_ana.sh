#!/bin/bash

SPEDIR=Eta6_Phi8_ter
INPUTDIR=/gpfs/ddn/cms/user/martini/AM/PatternRecognition/BE_5D/${SPEDIR}
OUTPUTDIR=/gpfs/ddn/cms/user/martini/AM/PatternRecognitionAnalysis/BE_5D/${SPEDIR}

mkdir -p ${OUTPUTDIR}

#for f in ${INPUTDIR}/*lo?midhig*.root
for f in ${INPUTDIR}/*lo2midhig_sec16_ss32_cov0.7_dc2_fk0*.root
do
    echo processing ${f}
    fb=`basename ${f} .root | cut -d_ -f-12`
    cd /gpfs/ddn/cms/user/martini/AM/CMSSW_6_1_2_SLHC6_patch1/src/Extractors/RecoExtractor/test/SectorMaker/
    ./AM_ana -c PR_eff -i /gpfs/ddn/cms/user/martini/AM/Sectors/Eta6_Phi8_sector_map.csv -f /gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MU/MU_612_SLHC6.root -o ${OUTPUTDIR}/${fb}_PRA.root -d ${INPUTDIR}/${fb}.root -n 100000000 

done

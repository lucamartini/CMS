#!/bin/bash

SECID=0
NH=5

SPEDIR=Eta6_Phi8_ter
INPUTDIR=/gpfs/ddn/cms/user/martini/AM/PatternRecognitionAnalysis/BE_5D/${SPEDIR}
OUTDIR=${INPUTDIR}/plots
mkdir -p ${OUTDIR}

for f in ${INPUTDIR}/*lo?midhig*.root
do
	if [[ $f == *sec0* ]]
	then
		let NH=7
		SECID=0
	elif [[ $f == *sec8* ]]
	then
		let NH=8
		SECID=8
	elif [[ $f == *sec16* ]]
	then
		let NH=6
		SECID=16
	else
		echo WRONG
		break
	fi
	
	echo "file ${f} , sec = ${SECID} , layers = ${NH}"

    echo "{ gROOT->ProcessLine(\".L do_pattern_effs.C+\"); gROOT->ProcessLine(\"do_pattern_effs(\\\"${f}\\\",${SECID},${NH})\"); }" > run.C
    root -b -q run.C

#    root -b -q 'do_pattern_effs.C("'"${f}"'",'"${SECID}"','"${NH}"')'
    fb=`basename ${f} .root`
	echo "moving Pattern_eff_sector_${SECID}_${NH}_hits_track.png"
    mv Pattern_eff_sector_${SECID}_${NH}_hits_track.png ${OUTDIR}/${fb}_PatternEff_NH${NH}.png
done

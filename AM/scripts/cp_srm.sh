#!/bin/bash

voms-proxy-init -voms cms

SRMADDRESS=srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=//cms/store/user/lmartini/AM/PatternRecognition/BE_5D
PATTERNDIR=Eta6_Phi8_ter
TEMPDIR=/gpfs/ddn/cms/user/martini/AM/CMSSW_6_1_2_SLHC6_patch1/src/DataProduction/batch/temp
mkdir -p ${TEMPDIR}

for f in `srmls ${SRMADDRESS}/${PATTERNDIR} | grep 'PIM_612' cut -d/ -f10 `
do
	# if [[ "${f}" == MU_612_SLHC6._612_SLHC6_MUBANK_lo2midhig_sec8_ss64_cov0.4_dc2_fk2.* ]]
	# then
		
  		echo "copying ${f} in ${TEMPDIR}"
  		lcg-cp -b -D srmv2 "${SRMADDRESS}/${PATTERNDIR}/${f}" file:///${TEMPDIR}/${f}
	# fi
done

for g in ${TEMPDIR}/*_150000_200000.root
do
    gb=`basename ${g} | cut -d_ -f-12`
	echo "merging ${gb}"
	mkdir -p /gpfs/ddn/cms/user/martini/AM/PatternRecognition/BE_5D/${PATTERNDIR}/
    hadd -f -k /gpfs/ddn/cms/user/martini/AM/PatternRecognition/BE_5D/${PATTERNDIR}/${gb}.root ${TEMPDIR}/${gb}_*.root
done

rm ${TEMPDIR}/*.root

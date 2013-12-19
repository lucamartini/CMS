#!/bin/bash

SPEDIR=Eta6_Phi8_ter

for f in `srmls srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=//cms/store/user/lmartini/AM/PatternRecognition/BE_5D/${SPEDIR}/ | grep 'PIM_' | cut -d/ -f10 `
do
  base=`basename $f`
  echo "removing srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=//cms/store/user/lmartini/AM/PatternRecognition/BE_5D/${SPEDIR}/${f}"
  srmrm -2 "srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=//cms/store/user/lmartini/AM/PatternRecognition/BE_5D/${SPEDIR}/${f}"

done


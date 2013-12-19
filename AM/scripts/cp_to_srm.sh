#!/bin/bash

for f in PatternBanks/BE_5D/Eta6_Phi8/*.pbk
do
  base=`basename $f`
  echo $f
  lcg-cp -b -D srmv2 file:////gpfs/ddn/cms/user/martini/AM/$f "srm://stormfe1.pi.infn.it:8444/srm/managerv2?SFN=/cms/store/user/lmartini/AM/PatternBanks/BE_5D/Eta6_Phi8/$base"
done


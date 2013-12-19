#!/bin/bash

# merges low, mid, hig banks of each sector
# input is the coverage, default is 0.5

BANKDIR=/gpfs/ddn/cms/user/martini/AM/PatternBanks/BE_5D/Eta6_Phi8_ter
BANKROOT=612_SLHC6_MUBANK
COV=cov0.5
if [ "$#" = 1 ]
then
    COV=cov$1
fi

COVhig=0.7
COVmid=0.7
COVlow=0.4

I=0

# for f in $BANKDIR/${BANKROOT}_PT_*.pbk
# do
# 	fb=`basename ${f}  | cut -d_ -f 5-`
# 	mv ${f} $BANKDIR/${BANKROOT}_${fb}
# done

for f in $BANKDIR/${BANKROOT}_hig_*${COVhig}*.pbk
do
	echo "----------->"
    fb=`basename ${f}  | cut -d_ -f 5-`
    froot=`basename ${f}  | cut -d_ -f -3`
    for g in $BANKDIR/${BANKROOT}_mid_*${COVmid}*.pbk
    do
        gb=`basename ${g}  | cut -d_ -f 5-`
        o=$BANKDIR/${froot}_midhig_${fb}
        o2o=$BANKDIR/${froot}_lo2midhig_${fb}
        o3o=$BANKDIR/${froot}_lo3midhig_${fb}

        if [ "$fb" == "$gb" ]
        then
            cd /gpfs/ddn/cms/user/martini/AM/CMSSW_6_1_2_SLHC6_patch1/src/amsimulation
            ./AMSimulation --MergeBanks --inputFile $f --secondFile $g --outputFile $o

            for h in $BANKDIR/${BANKROOT}_low2_*${COVlow}*.pbk 
            do
                hb=`basename ${h}  | cut -d_ -f 5-`
				hfirst=`basename ${h}  | cut -d_ -f 5-6`
				hsecond=`basename ${h}  | cut -d_ -f 8-`
                if [[ "${fb}" == ${hfirst}_cov${COVhig}_${hsecond} ]]  # <===== WARNING
                then
                    ./AMSimulation --MergeBanks --inputFile $o --secondFile $h --outputFile $o2o
                    printf "\n"
					((I++))
                fi
            done
  
            for h in $BANKDIR/${BANKROOT}_low3_*${COVlow}*.pbk
            do
                hb=`basename ${h}  | cut -d_ -f 5-`
				hfirst=`basename ${h}  | cut -d_ -f 5-6`
				hsecond=`basename ${h}  | cut -d_ -f 8-`
				if [[ "${fb}" == ${hfirst}_cov${COVhig}_${hsecond} ]]  # <===== WARNING
                then
                    ./AMSimulation --MergeBanks --inputFile $o --secondFile $h --outputFile $o3o
                    printf "\n"
					((I++))
                fi
            done
        fi
    done
	echo "<-----------"
done

echo "${I} merged"

cd $BANKDIR
rm *_midhig_*.pbk

echo merged

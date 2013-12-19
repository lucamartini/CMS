#!/bin/bash

# run bank generation 12 at time, 4 in a row --> 12 cpu needed
# input sets coverage (default sets 0.4 0.7 0.7)

cd ../CMSSW_6_1_2_SLHC6_patch1/src/amsimulation

# configurations
SS=32
DC=0
COV=0.5
if [ "$#" = 1 ]
then
    COV=$1
fi

FK=0
SEC=0
ActiveLayers="5 6 7 8 9 10"

# input tracks
PTMIN=2
PTMAX=100
PTstring=low2
INPUTDIR=/gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MUBANK_hig/

BANKNAME=612_SLHC6_MUBANK_hig_sec16_ss32_cov40_dc0_fk0.pbk

I=0

for SEC in 0 8 16
do
    for DC in 2 3
    do
        for SS in 32 64
        do
            fullnohup=""
            J=0
            # for PT in 2 3 5 20
            # for PT in 2 3
            for PT in 5 20
            do
				PTMIN=2
                if  [ "$PT" = 2 ]
                then
					if [ "$#" = 0 ]
					then
						COV=0.4
					fi
                    PTMIN=2
                    PTMAX=5
                    PTstring=low2
                    INPUTDIR=/gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MUBANK_low/
                fi
                if  [ "$PT" = 3 ]
                then
					if [ "$#" = 0 ]
					then
						COV=0.4
					fi
                    PTMIN=3
                    PTMAX=5
                    PTstring=low3
                    INPUTDIR=/gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MUBANK_low/
                fi
                if  [ "$PT" = 5 ]
                then
					if [ "$#" = 0 ]
					then
						COV=0.7
					fi
                    PTMIN=5
                    PTMAX=20
                    PTstring=mid
                    INPUTDIR=/gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MUBANK_mid/
                fi
                if  [ "$PT" = 20 ]
                then
					if [ "$#" = 0 ]
					then
						COV=0.7
					fi				
                    PTMIN=20
                    PTMAX=100
                    PTstring=hig
                    INPUTDIR=/gpfs/ddn/srm/cms/store/user/lmartini/AM/612_SLHC6_MUBANK_hig/
                fi

                if [ "$SEC" = 0 ]
                then
                    FK=1
                    ActiveLayers="5 6 18 19 20 21 22"
                fi
                if [ "$SEC" = 8 ]
                then
                    FK=2
                    ActiveLayers="5 6 7 8 9 10 18 19"
                fi
                if [ "$SEC" = 16 ]
                then
                    FK=0
                    ActiveLayers="5 6 7 8 9 10"
                fi

                ((I++))
                ((J++))

                INFOSTRING=PT_${PTstring}_sec${SEC}_ss${SS}_cov${COV}_dc${DC}_fk${FK}
                BANKNAME=612_SLHC6_MUBANK_${INFOSTRING}.pbk


               # echo ./AMSimulation --generateBank --coverage=${COV} --ss_size=${SS} --dc_bits=${DC} --pt_min=${PTMIN} --pt_max=${PTMAX} --maxFakeSStrip=${FK} --input_directory=${INPUTDIR} --sector_id=${SEC} --bank_name=${BANKNAME} --active_layers=\"${ActiveLayers}\"
               # ./AMSimulation --generateBank --coverage=${COV} --ss_size=${SS} --dc_bits=${DC} --pt_min=${PTMIN} --pt_max=${PTMAX} --maxFakeSStrip=${FK} --input_directory=${INPUTDIR} --sector_id=${SEC} --bank_name=${BANKNAME} --active_layers=\"${ActiveLayers}\" > ${INFOSTRING}.txt &
               fullnohup+="nohup ./AMSimulation --generateBank --coverage=${COV} --ss_size=${SS} --dc_bits=${DC} --pt_min=${PTMIN} --pt_max=${PTMAX} --maxFakeSStrip=${FK} --input_directory=${INPUTDIR} --sector_id=${SEC} --bank_name=${BANKNAME} --active_layers=\"${ActiveLayers}\" > ${INFOSTRING}.txt"
                if [ $J -lt 2 ] # 4
                then
                    fullnohup+=" && "
                else
                    fullnohup+=" & "
                fi
            done
            echo ${fullnohup}
            eval ${fullnohup}
        done
    done
done

echo jobs are $I

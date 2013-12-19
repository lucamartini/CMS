#!/bin/bash

###########################################
#
# Main script for standalone pattern bank generation 
# and recognition jobs
#
# The jobs themselves are launched by PG.sh
#
# --> Usage for bank generation:
#
# This script uses the bank files stored on the following SE:
#
# srm://lyogrid06.in2p3.fr//dpm/in2p3.fr/home/cms/data/store/user/sviret/SLHC/GEN/ 
#
# If you are registered on the CMS VO you have access to it
#
# source launch_PG.sh BG p1 p2 p3 p4
# with:
# p1 : The SE subdirectory containing the bank root files
# p2 : The number of the sector you want to build the bank on
# p3 : The directory where you will store the bank files
# p4 : BATCH or nothing: launch lxbatch jobs or not
#
# The rest of the options are set on the lines below
# Please be aware that BG jobs use a great amount of RAM, therefore they might crash 
# on batch machine
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP IV)
# 
#
# --> Usage for standalone pattern recognition:
#
#
# source launch_PG.sh PR p1 p2 p3 p4 p5 p6
# with:
# p1 : The SE subdirectory containing the data file you want to analyze
# p2 : The number of the sector you want to test
# p3 : The directory where you will retrieve the bank files
# p4 : How many events you want to test in total? 
# p5 : How many events per job (should be below p3...)?
# p6 : BATCH or nothing: launch lxbatch jobs or not 
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V.1)
#
#
# --> Usage for CMSSW pattern recognition:
#
#
# source launch_PG.sh FASTPR p1 p2 p3 p4 p5 p6
# with:
# p1 : The SE subdirectory containing the data file you want to analyze
# p2 : Not used here as you will use all available bank files (put -1)
# p3 : The directory where you will retrieve the bank files
# p4 : How many events per data file? 
# p5 : How many events per job (should be below p3...)?
# p6 : BATCH or nothing: launch lxbatch jobs or not 
#
# For more details, and examples, have a look at:
# 
# http://sviret.web.cern.ch/sviret/Welcome.php?n=CMS.HLLHCTuto (STEP V.2)
#
# Author: S.Viret (viret@in2p3.fr)
# Date  : 24/09/2013
#
# Script tested with release CMSSW_6_1_2_SLHC6_patch1
#
###########################################


# Here we retrieve the main parameters for the job 

STEP=${1}   # PR or BG or FASTPR
MATTER=${2}   # Directory where the input root files are
SECID=${3}   # The sector number
BANKDIR=${4}   # Directory where the bank (.pbk) files are
#BANKDIR=/cms/store/user/lmartini/AM/PatternBanks/BE_5D/Eta6_Phi8/
NTOT=${5}   # How many events per data file (PR step only)?
NPFILE=${6}   # How many events per job (PR step only)?

###################################
#
# The list of parameters you can modify is here
#
###################################

# You have to adapt this to your situation

# Useful for all steps

# The SE directory containing the input data (bank/samples/...)
INDIR=/cms/store/user/lmartini/AM/$MATTER
INDIR2=/gpfs/ddn/srm/cms/store/user/lmartini/AM/$MATTER

# The SE directory containing the PR output data
OUTDIR=/cms/store/user/lmartini/AM/PatternRecognition/BE_5D/Eta6_Phi8_ter

LFC_HOST="stormfe1.pi.infn.it:8444/srm/managerv2?SFN="
INDIR_GRID=srm://$LFC_HOST/$INDIR

INDIR_XROOT=$INDIR2
OUTDIR_GRID=srm://$LFC_HOST/$OUTDIR

# Following lines suppose that you have a certificate installed on lxplus. To do that follow the instructions given here:
#
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideLcgAccess
#
source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
voms-proxy-init --voms cms --valid 100:00 -out $HOME/.globus/gridproxy.cert
X509_USER_PROXY=${HOME}'/.globus/gridproxy.cert'

# Then we setup some environment variables

cd  ..
PACKDIR=`pwd`           # This is where the package is installed
cd  ../..
RELEASEDIR=`pwd`           # This is where the release is installed

cd $PACKDIR/batch

if [ "$STEP" == "FASTPR" ]
then

    echo 'The data will be read from directory: '$INDIR_XROOT
    echo 'The pattern reco output files will be written in: '$OUTDIR_GRID/

    srmmkdir -2 "$OUTDIR_GRID/"
    srmls $OUTDIR_GRID/
    mkdir -p logs 

    for l in `srmls $INDIR_GRID/ | cut -d/ -f8`        # for each input file
    do
        echo 'Working with file '$l

        for k in ${BANKDIR}/*lo?midhig*.pbk          # for each bank
        do
            kbase=`basename ${k}`
            echo 'Working with bank file '$kbase

            i=0
            j=$NPFILE

			while [ $i -lt $NTOT ]
			do
				lb=`basename ${l} root`
				kb=`basename ${k} pbk`
				OUT=${lb}_${kb}_${i}_${j}

				echo "#\!/bin/bash" > fpr_job_${OUT}.sh
				echo "echo executing: " >> fpr_job_${OUT}.sh
				echo "source $PACKDIR/batch/PG_Pisa.sh FASTPR file:${INDIR_XROOT}/$l $BANKDIR/$kbase $OUT.root  ${i} $NPFILE $OUTDIR_GRID $RELEASEDIR 1" >> fpr_job_${OUT}.sh
				chmod 755 fpr_job_${OUT}.sh

				if [ "${7}" == "BATCH" ]
				then
					QUI=`pwd`
					bsub -q local -e ${QUI}/logs/${LOGNAME}_${OUT}_err.txt -o ${QUI}/logs/${LOGNAME}_${OUT}_out.txt ${QUI}/fpr_job_${OUT}.sh
				fi

				i=`expr $i + $NPFILE`
				j=`expr $j + $NPFILE`

			done
		done
	done
fi


#!/bin/bash

rm data/z_eta_resolutions.csv;
echo "# xm,xp,z_rms,z_rms_err,eta_rms,eta_rms_err" > data/z_eta_resolutions.csv
./RetinaRZ -2 -n 10000 -xp 11 -xm 11
./RetinaRZ -2 -n 10000 -xp 21 -xm 21
./RetinaRZ -2 -n 10000 -xp 31 -xm 31
./RetinaRZ -2 -n 10000 -xp 41 -xm 41
./RetinaRZ -2 -n 10000 -xp 51 -xm 51
./RetinaRZ -2 -n 10000 -xp 61 -xm 61
./RetinaRZ -2 -n 10000 -xp 71 -xm 71
./RetinaRZ -2 -n 10000 -xp 81 -xm 81
./RetinaRZ -2 -n 10000 -xp 91 -xm 91
./RetinaRZ -2 -n 10000 -xp 101 -xm 101

cat data/z_eta_resolutions.csv

./Res


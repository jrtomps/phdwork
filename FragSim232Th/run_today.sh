#!/bin/bash

# $1 is the base name of the macro e.g. for frag_0.mac --> $1 = frag_0
# $2 is the iteration number to begin with
# $3 is the iteration number to end with

./run.sh frag_2_gx2_gy2_gz0_r3_r5_x1_y0_z0 0 3
./sendemail.py

cd analysis
echo "y" > tmpin
echo "../sim_output/frag_2_gx2_gy2_gz0_r3_r5_x1_y0_z0_" >> tmpin
echo "0" >> tmpin
echo "3" >> tmpin
./analyze_all < tmpin
./move_lambda_results frag_2_gx2_gy2_gz0_r3_r5_x1_y0_z0_fxd2_4M
cd ..


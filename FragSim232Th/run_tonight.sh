#!/bin/bash

# $1 is the base name of the macro e.g. for frag_0.mac --> $1 = frag_0
# $2 is the iteration number to begin with
# $3 is the iteration number to end with

#OUTPUTDIR="/media/jeromy/FreeAgent\ Drive/."
OUTPUTDIR=$PWD/sim_output
./run.sh frag_gx2_gy0_gz0_r1_r5_x1_y0_z0 1 2
mv sim_output/frag_gx2_gy0_gz0* $OUTPUTDIR

#./run.sh frag_gx2_gy0_gz0_r1_r5_x1_y0_z0 3 4
#mv sim_output/frag_gx2_gy0_gz0* $OUTPUTDIR


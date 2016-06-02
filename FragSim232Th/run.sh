#!/bin/bash -e

# $1 is the base name of the macro e.g. for frag_0.mac --> $1 = frag_0
# $2 is the iteration number to begin with
# $3 is the iteration number to end with

name=$1
SIMNAME=fragsim232th
OUTDIR=sim_output

echo ${name}_$2.seed > temp;
${SIMNAME} macros/${name}.mac < temp
mv ${OUTDIR}/${name}.root ${OUTDIR}/${name}_$2.root

start=$(($2+1));

for index in $(seq $start 1 $3) ; 
do 
	echo ${name}_${index}.seed > temp;
	echo Y >> temp;
	${SIMNAME} macros/${name}.mac < temp
	mv ${OUTDIR}/${name}.root ${OUTDIR}/${name}_${index}.root
done

./sendemail.py

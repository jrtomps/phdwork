#!/bin/bash

echo "Do you want visualization? (y/n)";
read ans;

if [ "$ans" == "y" ] ; then
	if [ "${G4VIS_USE}x" == "x" ] ; then
	# -v returns true if the variable has been set
#	if [ ! -v G4VIS_USE ] ; then 
		export G4VIS_USE=1 
	fi
	if [ "${G4_USE_QT}x" == "x" ] ; then
		export G4_USE_QT=1 
	fi
	if [ "${G4UI_USE_QT}x" == "x" ] ; then
		export G4UI_USE_QT=1 
	fi
	if [ "${G4_USE_OPENGL}x" == "x" ] ; then
		export G4_USE_OPENGL=1 
	fi
	if [ "${G4VIS_BUILD_OPENGLQT_DRIVER}x" == "x" ] ; then
		export G4VIS_BUILD_OPENGLQT_DRIVER=1 
	fi
	unset G4UI_USE_TCSH
else
	echo "Visualization turned off" 
	unset G4VIS_USE
	unset G4_USE_QT
	unset G4UI_USE_QT
	unset G4_USE_OPENGLQT
	unset G4VIS_BUILD_OPENGLQT_DRIVER
	if [ "${G4UI_USE_TCSH}x" == "x" ] ; then
		export G4UI_USE_TCSH=1
	fi
fi

export G4LEDATA=/usr/local/geant4/geant4.9.6.p04-build/data/G4EMLOW6.32
export SIM_OUTPUT_DIR="/mnt/PhD/sim_output"
#export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$HOME/g4work/tmp/Linux-g++/FragSim02

#!/bin/bash

SIM_NAME=FragSim232Th

export SIM_OUT_DIR=${G4WORKDIR}/${SIM_NAME}/sim_output

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${G4WORKDIR}/tmp/Linux-g++/${SIM_NAME}

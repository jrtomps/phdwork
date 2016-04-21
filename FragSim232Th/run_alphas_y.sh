#!/bin/bash

echo "y=-5mm"
FragSim232Th macros/alpha_source_x0_yn5mm_z0.mac < ar0
echo "y=-3mm"
FragSim232Th macros/alpha_source_x0_yn3mm_z0.mac < ar1
#echo "y=0mm"
#FragSim232Th macros/alpha_source_x0_y0_z0.mac < ar2
echo "y=3mm"
FragSim232Th macros/alpha_source_x0_y3mm_z0.mac < ar3
echo "y=5mm"
FragSim232Th macros/alpha_source_x0_y5mm_z0.mac < ar4
echo "y=10mm"
FragSim232Th macros/alpha_source_x0_y10mm_z0.mac < ar5


#!/bin/bash

echo "x=-5mm"
FragSim232Th macros/alpha_source_xn5mm_y0_z0.mac < ar0
echo "x=-3mm"
FragSim232Th macros/alpha_source_xn3mm_y0_z0.mac < ar1
echo "x=0mm"
FragSim232Th macros/alpha_source_x0_y0_z0.mac < ar2
echo "x=3mm"
FragSim232Th macros/alpha_source_x3mm_y0_z0.mac < ar3
echo "x=5mm"
FragSim232Th macros/alpha_source_x5mm_y0_z0.mac < ar4
echo "x=10mm"
FragSim232Th macros/alpha_source_x10mm_y0_z0.mac < ar5


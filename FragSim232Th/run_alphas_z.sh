#!/bin/bash

echo "z=-5mm"
FragSim232Th macros/alpha_source_x0_y0_zn5mm.mac < ar0
echo "z=-3mm"
FragSim232Th macros/alpha_source_x0_y0_zn3mm.mac < ar1
echo "z=3mm"
FragSim232Th macros/alpha_source_x0_y0_z3mm.mac < ar3
echo "z=5mm"
FragSim232Th macros/alpha_source_x0_y0_z5mm.mac < ar4
echo "z=10mm"
FragSim232Th macros/alpha_source_x0_y0_z10mm.mac < ar5


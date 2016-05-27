#!/bin/bash

echo "Choose analysis session.";
echo "1 - Narrow";
echo "2 - Wide";

read choice;

case $choice in
1)
    rm -f fnameconfig.dat;
    ln -s fnameconfig_narrow.dat fnameconfig.dat;
#	rm -f configdatabase.dat;
#   rm -f pffrag2_hist_run;
#	rm -f results;
#	ln -s configdatabase_narrow.dat configdatabase.dat;
#	ln -s pffrag2_hist_run_narrow pffrag2_hist_run;
#	ln -s results_narrow results;
	;;
2)
    rm -f fnameconfig.dat;
    ln -s fnameconfig_wide.dat fnameconfig.dat;
#	rm -f configdatabase.dat;
#   rm -f pffrag2_hist_run;
#	rm -f results;
#	ln -s configdatabase_wide.dat configdatabase.dat;
#	ln -s pffrag2_hist_run_wide pffrag2_hist_run;
#	ln -s results_wide results;
	;;
*)
#	echo "Invalid entry";
	;;
esac



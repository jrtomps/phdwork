
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "Rtypes.h"
#include "Plane.h"
#include "ReducedDimFunction.h"
#include "RegionIntegratorMultiDim.h"

int main()
{
    std::string fname;

    std::cout << "enter file name : ";
    std::getline(std::cin, fname);

    std::ifstream ifile(fname.data(),std::ios::in);
    RegionFileLoader rfl(ifile);
    ifile.close();

    std::cout << "Found " << rfl.GetRegions().size() << " boundaries" << std::endl;

    std::vector<Double_t> pars(4,0);
    pars[2] = 1;
    pars[3] = 1;

    Plane plane(pars);
    RegionIntegratorMultiDim rimd;
    rimd.SetFunction(plane);

    Double_t result = rimd.RegionIntegral(rfl.GetRegions().front());
    std::cout << "integral (grid) = " << result << std::endl;
    rimd.SetUsingCenter(true);
    result = rimd.RegionIntegral(rfl.GetRegions().front());
    std::cout << "integral (cntr) = " << result << std::endl;

    std::cout << "ideally should be = " << ::acos(-1)*9.0 << std::endl;

    return 0;
}



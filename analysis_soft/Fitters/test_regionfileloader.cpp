#include <iostream>
#include <fstream>
#include <string>
#include "RegionIntegratorMultiDim.h"

int main()
{
    std::string fname;

    std::cout << "enter file name : ";
    std::getline(std::cin, fname);

    std::ifstream ifile(fname.data(),std::ios::in);
    RegionFileLoader rfl(ifile);
    ifile.close();

    std::ofstream of("testregion_out", std::ios::out);
    const std::vector<Regions>& regs = rfl.GetRegions();
    for (UInt_t i=0; i<regs.size(); i++)
    {
        of << "det" << i << regs[i] << std::endl;
    }
    of.close();

    std::cout << "Found " << rfl.GetRegions().size() << " boundaries" << std::endl;


    return 0;
}



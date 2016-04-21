
#include <dirent.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "Math/WrappedFunction.h"
#include "RegionIntegratorMultiDim.h"
#include "Math/IntegratorMultiDim.h"
#include "SphCoordsIntegrand.h"
#include "Plane.h"

void ListAllFilesInDirOfType(std::string dirname, std::string fsuffix)
{

    ::DIR* dp = ::opendir(dirname.data());

    ::dirent* dir = 0;
    while ( (dir = ::readdir(dp)) )
    {
        std::string name(dir->d_name);
        if (name.rfind(fsuffix)!=std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
    }

    std::cout << std::endl;
}


std::string GetRegionFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".bndry");
    std::cout << "\nEnter location of regions : ";
    std::getline(std::cin, fname);
    return fname;
}

Double_t Unity (const Double_t*) { return 1.0;}

Int_t Main()
{

    std::string fname;
    std::cout << "\nFor the regions ";
    ListAllFilesInDirOfType (".",".bndry");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);

    std::ifstream ifile(fname.data(),std::ios::in);
    RegionFileLoader rfl(ifile);
    ifile.close();
    std::cout << "Found " << rfl.GetRegions().size() << " boundaries" << std::endl;

    UInt_t in;
    std::cout << "\nEnter index of region to integrate : ";
    std::cin >> in;
    Regions reg(rfl.GetRegions()[in]);
    reg *= CLHEP::deg;

    RegionIntegratorMultiDim integ;
    ROOT::Math::WrappedMultiFunction<> r(&Unity,2);

    char answer;
    std::cout << "Is this integration over spherical coords? (y/n)";
    std::cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        SphCoordsIntegrand r_sin(r);

        integ.SetFunction(r_sin);
    }
    else
    {
//        std::vector<Double_t> pars(4,0);
//        pars[2] = 1;
//        pars[3] = 1;
//        Plane pl(pars);
        integ.SetFunction(r);
    }

    Double_t result = integ.RegionIntegral(reg);

    std::cout << "\n" << std::setw(20) << "Result" << " = " << result;
    std::cout << "\n" << std::setw(20) << "TCutG::Area()" << " = " << regions_cast<TCutG>(reg).Area();

    std::cout << std::endl;
    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}

#endif

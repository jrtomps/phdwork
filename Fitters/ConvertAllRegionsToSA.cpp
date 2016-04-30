

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

    std::string res_fname;
    std::cout << "\nEnter file to write output to : ";
    std::getline(std::cin,res_fname);

    std::ofstream ofile(res_fname.data());
    ofile << std::setiosflags(std::ios::scientific)
            << std::setprecision(2);
    ofile << "Results"
            << "\n--------";

    ROOT::Math::WrappedMultiFunction<> r(&Unity,2);
    SphCoordsIntegrand r_sin(r);
    RegionIntegratorMultiDim integ;

    integ.SetFunction(r_sin);

    for (UInt_t i=0; i<rfl.GetRegions().size(); i++)
    {
        Regions reg(rfl.GetRegions()[i]);
        reg *= CLHEP::deg;

        Double_t result = integ.RegionIntegral(reg);
        Double_t res_err = integ.Error()*reg.size();
        ofile << "\n" << std::setw(10) << result/(4.0*CLHEP::pi)
                << std::setw(10) << res_err/(4.0*CLHEP::pi);
    }

    ofile << std::endl;

    ofile.close();
    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}

#endif

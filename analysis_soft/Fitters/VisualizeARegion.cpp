

#include <dirent.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "TApplication.h"
#include <CLHEP/Units/PhysicalConstants.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include "RegionIntegratorMultiDim.h"

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
    TApplication theapp("theapp",0,0);

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
    std::cout << "\nEnter index of region to visualize : ";
    std::cin >> in;
    Regions reg(rfl.GetRegions()[in]);
    reg *= CLHEP::deg;

    TCutG mycut = regions_cast<TCutG>(reg);

    mycut.Print();
    mycut.Draw("ALP");

    theapp.Run();
    return 0;
}

#ifndef __CINT__
int main()
{
    return Main();
}

#endif

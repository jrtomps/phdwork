

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <cmath>
#include "Rtypes.h"
#include "AngDist.h"
#include "RegionIntegralChiSq.h"
#include "TGraph2DErrors.h"
#include "TFile.h"
#include "Parameters.h"

TGraph2DErrors* GetGraph(std::string fname)
{
    TGraph2DErrors* gr = 0;

    std::string fGrName = "mygraph";

    TFile *f = new TFile(fname.data());
    if (!f->IsOpen())
    {
        std::cerr << "Failed to open " << fname << std::endl;
        return gr;
    }

    std::cout << "\n";  f->ls();

    std::cout << "\nEnter name of graph : ";
    std::getline(std::cin, fGrName);

    f->GetObject(fGrName.data(), gr);
    if (gr!=NULL)
    {

        TGraph2DErrors* tgr = (TGraph2DErrors*) gr->Clone("mygraph_0");
        gr=tgr;
        gr->SetDirectory(0);
    }

    return gr;
}

void DisplayAllFilesInDirOfType (std::string dr, std::string suffix)
{
    ::DIR* dp = ::opendir(dr.data());
    ::dirent* dir;

    std::string name;

    std::cout << std::left
            << "\nThe following .dats file are in the current folder\n";
    while( (dir = ::readdir(dp) ) )
    {
        name = dir->d_name;
        if (name.rfind(suffix)!= std::string::npos)
        {
            std::cout << "\n\t" << name;
        }
    }
    ::closedir(dp);

    std::cout << std::right << std::endl;
}

int main()
{
    std::string fname;
    std::cout << "\nFor the graph ";
    DisplayAllFilesInDirOfType (".",".root");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);
    TGraph2DErrors *mygr = GetGraph(fname);

    std::cout << "\nFor the regions ";
    DisplayAllFilesInDirOfType (".",".bndry");
    std::cout << "\nEnter filename : ";
    std::getline(std::cin, fname);

    std::ifstream ifile(fname.data(),std::ios::in);
    RegionFileLoader rfl(ifile);
    ifile.close();
    std::cout << "Found " << rfl.GetRegions().size() << " boundaries" << std::endl;


    std::cout << "\nFor the parameters ";
    DisplayAllFilesInDirOfType (".",".params");
    std::cout << "\nEnter param file : ";
    std::getline(std::cin, fname);
    ifile.open(fname.data(),std::ios::in);
    Parameters params(ifile);
    ifile.close();

    std::cout << "\nFound the following parameters:"
            << "\n" << params
            << std::endl;

    AngDist w(params);
    RegionIntegralChiSq rics(mygr, w, rfl.GetRegions());
    std::map<UInt_t, Int_t> mmap = rics.GetPointToRegionMap();
    std::cout << "mmap.size()=" << mmap.size() << std::endl;
    std::cout << "\nPointToRegionMap" << std::endl;
    std::cout << mmap << std::endl;



    return 0;
}



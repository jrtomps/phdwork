

#include <iostream>
#include <string>
#include <dirent.h>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "Rtypes.h"
#include "TFile.h"
#include "TGraph2DErrors.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "AngDistY.h"
#include "Parameters.h"
#include "Math/IParamFunction.h"
#include "TF2.h"
#include "Math/IntegratorMultiDim.h"
#include "TMath.h"
#include "ParamFunctorAdapter.h"


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


std::string GetParamFile()
{
    std::string fname;
    ListAllFilesInDirOfType(".",".params");
    std::cout << "\nEnter location of params : ";
    std::getline(std::cin, fname);
    return fname;
}

Parameters CreateParamsManually()
{

    Double_t val;
    Parameters pars;
    std::cout << "\nc00 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(0,val,"c00",0));

    std::cout << "c20 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(1,val,"c20",0));

    std::cout << "c22 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(2,val,"c22",0));

    std::cout << "c40 = ";
    std::cin >> val;
    pars.AddParameter(Parameter(3,val,"c40",0));

    return pars;
}


Int_t VisualizeSurface()
{
    Parameters params;

    Int_t choice;
    std::cout << "Enter params "
            << "\n0 - manually"
            << "\n1 - from file"
            << std::endl;
    std::cin >> choice;
    if (choice==0)
    {
        params = CreateParamsManually();
    }
    else
    {
        if (std::cin.peek()=='\n') std::cin.get();
        std::string parfname = GetParamFile();
        std::ifstream pfile(parfname.data(),std::ios::in);
        params = Parameters(pfile);
        pfile.close();
    }


    if (! params.KeysAreSensible()) return -1;

    std::cout << "\nOriginal parameters :" << std::endl;
    std::cout << params << std::endl;

    Parameters cpars = AngDistY::ConvertToStandardParameters(params);


    std::cout << "\nConverted parameters" << std::endl;
    std::cout << cpars << std::endl;


    return 0;
}


Int_t main()
{

    Int_t ret = VisualizeSurface();


    return ret;
}


